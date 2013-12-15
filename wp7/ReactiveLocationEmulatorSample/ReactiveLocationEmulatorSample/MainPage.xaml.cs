using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Threading;
using System.IO;
using System.IO.IsolatedStorage;
using Microsoft.Phone.Reactive;
// Microsoft.Phone.Shell provides access to the ApplicationBar
using Microsoft.Phone.Shell;
// System.Device.Location provides access to the Location Service
using System.Device.Location;

namespace ReactiveLocationEmulatorSample
{
    public partial class MainPage : PhoneApplicationPage
    {
        // GeocoordinateWatcher for accessing the Location Service
        GeoCoordinateWatcher watcher;

        // useEmulation determines whether live or emulated data is used
        bool useEmulation = false;

        // A static bool to keep track of whether data acquisition is running or not.
        static bool running = false;

        // Hard coded array for emulated status values
        static GeoPositionStatus[] emulatedStatusValues = { GeoPositionStatus.Initializing, GeoPositionStatus.Ready };
        // Delay after each emulated status value in the array above. These two arrays should be the same length.
        static int[] emulatedStatusDelay = { 1000, 1000 };

        // Threads for position and status emulation and a static bool
        Thread positionEmulationThread;
        Thread statusEmulationThread;

        // sampleInterval determines how often, in milliseconds, the location data stream is sampled
        double sampleInterval = 100;

        // When recordPosition is enabled, positions are stored in geoCoordinateList
        bool recordPosition = false;
        List<GeoCoordinate> geoCoordinateList;

        // Icon buttons for the ApplicationBar
        ApplicationBarIconButton pauseButton;
        ApplicationBarIconButton startButton;
        ApplicationBarIconButton emulationButton;
        ApplicationBarIconButton recordButton;
        ApplicationBarIconButton deleteButton;



        public MainPage()
        {
            InitializeComponent();

            SupportedOrientations = SupportedPageOrientation.Portrait | SupportedPageOrientation.Landscape;

            // Initialize ApplicationBar
            ApplicationBar appBar = new ApplicationBar();
            appBar.IsVisible = true;
            this.ApplicationBar = appBar;

            // startButton starts data acquisition
            startButton = new ApplicationBarIconButton(new Uri("Buttons/start.png", UriKind.Relative));
            startButton.Text = "start";
            startButton.Click += new EventHandler(startButton_Click);
            appBar.Buttons.Add(startButton);

            // pauseButton stops data acquisition. It replaces startButton while acquisition is running.
            pauseButton = new ApplicationBarIconButton(new Uri("Buttons/pause.png", UriKind.Relative));
            pauseButton.Text = "pause";
            pauseButton.Click += new EventHandler(pauseButton_Click);

            // emulationButton toggles between live and emulated data.
            emulationButton = new ApplicationBarIconButton(new Uri("Buttons/emulation.png", UriKind.Relative));
            emulationButton.Text = "emulate";
            emulationButton.Click += new EventHandler(emulationButton_Click);
            appBar.Buttons.Add(emulationButton);

            // recordButton toggles recording position on and off
            recordButton = new ApplicationBarIconButton(new Uri("Buttons/record.png", UriKind.Relative));
            recordButton.Text = "record";
            recordButton.Click += new EventHandler(recordButton_Click);
            appBar.Buttons.Add(recordButton);

            // deleteButton allows the user to delete the saved data file. It is only displayed
            // when the saved data file exists.
            deleteButton = new ApplicationBarIconButton(new Uri("Buttons/delete.png", UriKind.Relative));
            deleteButton.Text = "delete";
            deleteButton.Click += new EventHandler(deleteButton_Click);

            // Check to seee if the data file exists. If so, add the deleteButton to the application bar
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            if (isoStore.FileExists("LocationData.txt"))
            {
                appBar.Buttons.Add(deleteButton);
            }


            // Set the informational TextBlocks to their initial value
            emulationTextBlock.Text = useEmulation.ToString();
            recordingTextBlock.Text = recordPosition.ToString();
            sampleSlider.Value = sampleInterval * .001;
            sampleLabel.Text = "sample interval: " + sampleSlider.Value.ToString("0.0") + " seconds";
        }
        /// <summary>
        /// Click handler for the startButton. If emulation is not used, the GeoCoordinateWatcher is
        /// instantiated, then an Observable sequence is obtained using the FromEvent method.
        /// The Sample method is used to filter the sequence. Then the resulting sequence is 
        /// subscribed to. If emulation is used, a new Thread is launched for the emulation.
        /// </summary>
        /// <param name="sender">Source of the click event</param>
        /// <param name="e">EventArgs for the event</param>
        void startButton_Click(object sender, EventArgs e)
        {
            if (!useEmulation)
            {
                // If emulation is not used, instantiate the GeoCoordinateWatcher
                watcher = new GeoCoordinateWatcher(GeoPositionAccuracy.High);

                // FromEvent is used to obtain an Observable sequence of GeoPositionChangedEventArgs
                var locationEventAsObservable = Observable.FromEvent<GeoPositionChangedEventArgs<GeoCoordinate>>(
                    ev => watcher.PositionChanged += ev,
                    ev => watcher.PositionChanged -= ev);

                // To make the emulation code less complicated, get a stream of GeoCoordinate objects
                // from the stream of GeoPositionChangedEventArgs provided by the watcher.PositionChanged event.
                var geoCoordinatesFromEventArgs = from ev in locationEventAsObservable
                                                  select ev.EventArgs.Position.Location;

                // Sample the observable to return only one position per sample interval. The interval can be
                // adjusted by the sampleSlider in the UI.
                var sampledLocationEvents = geoCoordinatesFromEventArgs.Sample(TimeSpan.FromMilliseconds(sampleInterval));

                // Subscribe to the filtered Observable stream. InvokePositionChanged will be called each
                // time a new GeoCoordinate arrives in the stream.
                sampledLocationEvents.Subscribe(coordinate => InvokePositionChanged(coordinate));


                // Create an observable sequence of GeoPositionStatusChanged events.
                var locationStatusEventAsObservable = Observable.FromEvent<GeoPositionStatusChangedEventArgs>(
                    ev => watcher.StatusChanged += ev,
                    ev => watcher.StatusChanged -= ev);

                // For simplicity, create a stream of GeoPositionStatus objects from the stream of
                // GeoPositionStatusChangedEventArgs objects
                var statusFromEventArgs = from ev in locationStatusEventAsObservable
                                          select ev.EventArgs.Status;

                // Subscribe to the Observable  stream. InvokeStatusChanged will be called each time
                // a new GeoPositionStatus object arrives in the stream.
                statusFromEventArgs.Subscribe(status => InvokeStatusChanged(status));

                // Start the GeoCoordinateWatcher
                watcher.Start();
            }
            else
            {
                // If emulation is being used, start a new thread and run the StartStatusEmulation method.
                // When the emulated status becomes Ready, the position emulation thread is launched
                statusEmulationThread = new Thread(StartStatusEmulation);
                statusEmulationThread.Start();

            }

            // Replace the startButton with the pauseButton and set running to true
            this.ApplicationBar.Buttons[0] = pauseButton;
            running = true;
        }
        /// <summary>
        /// Click handler for the pauseButton. This stops data acquisition.
        /// </summary>
        /// <param name="sender">Source of the click event.</param>
        /// <param name="e">EventArgs</param>
        void pauseButton_Click(object sender, EventArgs e)
        {
            // If emulation is not being used, stop the GeoCoordinateWatcher
            if (!useEmulation)
            {
                watcher.Stop();
            }

            // Set the running state to false
            running = false;



            // Replace the pauseButton with the startButton.
            this.ApplicationBar.Buttons[0] = startButton;

            // Clear the contents of the statusTextBlock
            statusTextBlock.Text = "...";
        }
        /// <summary>
        /// Click handler for the emulationButton. This toggles between using live and emulated data.
        /// </summary>
        /// <param name="sender">The source of the click event</param>
        /// <param name="e">EventArgs</param>
        void emulationButton_Click(object sender, EventArgs e)
        {
            // For simplicity, the user can only toggle emulation on and off while
            // data acquisition is not running
            if (running)
            {
                return;
            }

            // Toggle the useEmulation value and update the emulationTextBlock
            useEmulation = !useEmulation;
            emulationTextBlock.Text = useEmulation.ToString();
        }
        /// <summary>
        /// Click handler for the recordButton. This button toggles whether location data is recorded.
        /// When recording is turned off, recorded data is saved to a file is Isolated Storage.
        /// </summary>
        /// <param name="sender">Source of the click event</param>
        /// <param name="e">EventArgs</param>
        void recordButton_Click(object sender, EventArgs e)
        {
            // Toggle the recordPosition variable and update the recordingTextBlock
            recordPosition = !recordPosition;
            recordingTextBlock.Text = recordPosition.ToString();

            // If recording has been turned on, initialize the coordinate list
            if (recordPosition)
            {
                geoCoordinateList = new List<GeoCoordinate>();
            }
            else
            {
                // If recording has been turned off, write the existing data to a file
                WriteLocationDataFile();

                // If the deleteButton was not already on the ApplicationBar, add it.
                if (!this.ApplicationBar.Buttons.Contains(deleteButton))
                {
                    this.ApplicationBar.Buttons.Add(deleteButton);
                }
            }
        }
        /// <summary>
        /// Click handler for the deleteButton. Allows the user to delete the location
        /// data file in Isolated Storage. When there is no Isolated Storage file, the
        /// emulator uses the resource file built into the .xap file, LocationData.txt 
        /// in the Solution Explorer ---->
        /// </summary>
        /// <param name="sender">The source of the click event</param>
        /// <param name="e">EventArgs</param>
        void deleteButton_Click(object sender, EventArgs e)
        {
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();
            if (isoStore.FileExists("LocationData.txt"))
            {
                isoStore.DeleteFile("LocationData.txt");
                this.ApplicationBar.Buttons.Remove(deleteButton);
            }
        }
        /// <summary>
        /// Called when the user adjusts the sampleSlider. The sample interval is set to the
        /// value (multiplied by 1000 for milliseconds). The data sample rate is only
        /// changed when data acquisition is started - not while it is running.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void sampleSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            sampleLabel.Text = "sample interval: " + sampleSlider.Value.ToString("0.0") + " seconds";
            sampleInterval = sampleSlider.Value * 1000;

        }
        /// <summary>
        /// Called from the startButton click handler when emulation is turned on. This method
        /// calls EmulateLocationEvents which returns an IEnumerable sequence of GeoCoordinates.
        /// ToObservable converts this stream to an Observable sequence. Once the Observable is
        /// created, the method is the same as the non-emulation method. The stream is sampled
        /// and then Subscribe registers a handler that is called when new location data arrives.
        /// </summary>
        void StartPositionEmulation()
        {
            var emulatedEventsToObservable = EmulatePositionEvents().ToObservable();

            var sampledEvents = emulatedEventsToObservable.Sample(TimeSpan.FromMilliseconds(sampleInterval));

            sampledEvents.Subscribe(coordinate => InvokePositionChanged(coordinate));

        }
        /// <summary>
        /// Called from the startButton click handler when emulation is turned on. This method
        /// calls EmulateStatusEvents which returns an IEnumerable sequence of GeoPositionStatus
        /// objects. ToObservable converts this stream to an Observable sequence. The stream is 
        /// subscribed to, registering a handler that is called when new status data arrives.
        /// </summary>
        void StartStatusEmulation()
        {
            var statusEventsToObservable = EmulateStatusEvents().ToObservable();

            var statusFromEventArgs = from s in statusEventsToObservable
                                      select s.Status;

            statusFromEventArgs.Subscribe(status => InvokeStatusChanged(status));
        }

        /// <summary>
        /// InvokePositionChanged is called when new location data is available (either live
        /// or emulated). It uses BeginInvoke to call another handler on the Page's UI thread.
        /// </summary>
        /// <param name="coordinate"></param>
        void InvokePositionChanged(GeoCoordinate coordinate)
        {
            Deployment.Current.Dispatcher.BeginInvoke(() => PositionChanged(coordinate));
        }
        /// <summary>
        /// This is where the application responds to new location data. In this case, it simply
        /// displays the location information
        /// </summary>
        /// <param name="coordinate"></param>
        void PositionChanged(GeoCoordinate coordinate)
        {
            // Display the latest location information
            latitudeTextBlock.Text = coordinate.Latitude.ToString("0.0000");
            longitudeTextBlock.Text = coordinate.Longitude.ToString("0.0000");

            // If recording is enabled, record the position
            if (recordPosition)
            {
                geoCoordinateList.Add(coordinate);
            }
        }
        /// <summary>
        /// InvokeStatusChanged is called when new location status is available (either live
        /// or emulated). It uses BeginInvoke to call another handler on the Page's UI thread.
        /// </summary>
        /// <param name="status"></param>
        void InvokeStatusChanged(GeoPositionStatus status)
        {
            Deployment.Current.Dispatcher.BeginInvoke(() => StatusChanged(status));
        }

        /// <summary>
        /// This is where the application responds to new status information. In this case, it simply
        /// displays the location information.  If emulation is enabled and the status is Ready, a
        /// new thread is launched for position emulation. This mimics the behaviour of the live data,
        /// where location data doesn't arrive until the LocationService status is Ready.
        /// </summary>
        /// <param name="status"></param>
        void StatusChanged(GeoPositionStatus status)
        {
            // Display the current status
            statusTextBlock.Text = status.ToString();

            // If emulation is being used and the status is Ready, start the position emulation thread
            if (status == GeoPositionStatus.Ready && useEmulation)
            {

                positionEmulationThread = new Thread(StartPositionEmulation);
                positionEmulationThread.Start();
            }
        }

        /// <summary>
        /// Delivers emulated position events. Coordinates are read from a data file and returned
        /// one at a time, through the IEnumerable interface.
        /// </summary>
        /// <returns></returns>
        static IEnumerable<GeoCoordinate> EmulatePositionEvents()
        {
            // Get a list of coordinates from a data file
            var coordinates = ReadLocationDataFile();

            // Loop through all of the coordinates
            int index = 0;
            for (; ; )
            {

                // Sleep for a moment. You can adjust this to change the rate at which the events fire
                Thread.Sleep(200);

                // Return the next coordinate in the list
                yield return coordinates[index];

                

                // Increment the array index
                index = (index + 1) % coordinates.Count;

                // Break out of the loop if running is turned off
                if (!running)
                {
                    break;
                }
            }
        }
        /// <summary>
        /// Returns the status values defined in the emulatedStatusValues variable and
        /// then sleeps for the amount of time specified in the emulatedStatusDelay variable
        /// </summary>
        /// <returns></returns>
        static IEnumerable<GeoPositionStatusChangedEventArgs> EmulateStatusEvents()
        {
            // Loop over the emulated status values until the end of the list
            for (int i = 0; i < emulatedStatusValues.Length; i++)
            {
                
                // return the current status value
                yield return new GeoPositionStatusChangedEventArgs(emulatedStatusValues[i]);

                // sleep for the specified delay for each status value in the array
                Thread.Sleep(emulatedStatusDelay[i]);

                // Break out of the loop if running is turned off
                if (!running)
                {
                    break;
                }
            }

        }

        /// <summary>
        /// Writes the location data to a file. Called when the user turns recording off. For
        /// simplicity, the coordinates are stored in comma-delimited text file.
        /// </summary>
        void WriteLocationDataFile()
        {
            // Open the isolated storage file
            using (var isoStore = IsolatedStorageFile.GetUserStoreForApplication())
            {
                // Open the file stream
                using (StreamWriter sw = new StreamWriter(new IsolatedStorageFileStream("LocationData.txt", FileMode.OpenOrCreate, isoStore)))
                {
                    // Loop through the coordinates in the list and write them to the file
                    foreach (GeoCoordinate g in geoCoordinateList)
                    {
                        sw.WriteLine(g.Latitude + "," + g.Longitude);
                    }
                    sw.Close();
                }
            }
        }

        /// <summary>
        /// This method is called when location emulation is started. It first attempts to read the file
        /// that users can save in isolated storage. If that file doesn't exist, the resource file packaged
        /// with the application is used, LocationData.txt in the Solution Explorer ---->
        /// </summary>
        /// <returns></returns>
        static List<GeoCoordinate> ReadLocationDataFile()
        {
            // Initialize the coordinate list
            var coordinates = new List<GeoCoordinate>();

            // Open the isolated storage file
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();

            Stream dataFileStream = null;

            // First, check for a file in isolated storage
            if (isoStore.FileExists("LocationData.txt"))
            {
                dataFileStream = new IsolatedStorageFileStream("LocationData.txt", FileMode.OpenOrCreate, isoStore);
            }
            else
            {
                // If there is no file in isolated storage, use the resource file
                Uri resourceUri = new Uri("/ReactiveLocationEmulatorSample;component/LocationData.txt", UriKind.Relative);
                System.Windows.Resources.StreamResourceInfo resourceInfo = Application.GetResourceStream(resourceUri);
                dataFileStream = resourceInfo.Stream;
            }

            // read the file and add the coordinates to the list
            using (StreamReader sr = new StreamReader(dataFileStream))
            {
                while (!sr.EndOfStream)
                {
                    string[] latlong = sr.ReadLine().Split(',');
                    coordinates.Add(new GeoCoordinate(Double.Parse(latlong[0]), Double.Parse(latlong[1])));
                }
                sr.Close();
            }
            return coordinates;
        }
    }
}
