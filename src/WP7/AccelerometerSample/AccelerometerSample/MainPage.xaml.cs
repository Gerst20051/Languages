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
using Microsoft.Devices.Sensors;
using Microsoft.Phone.Shell;

namespace AccelerometerSample
{
    public partial class MainPage : PhoneApplicationPage
    {
        Accelerometer accelerometer;

        #region Initialization

        /// <summary>
        /// Constructor for the PhoneApplicationPage object. 
        /// In this method, the Application Bar is initialized.
        /// </summary>
        public MainPage()
        {
            InitializeComponent();

            ApplicationBar = new ApplicationBar();
            ApplicationBar.IsVisible = true;

            ApplicationBarIconButton startStopButton = new ApplicationBarIconButton(new Uri("/Images/startstop.png", UriKind.Relative));
            startStopButton.Text = "on/off";
            startStopButton.Click += new EventHandler(startStopButton_Click);
            ApplicationBar.Buttons.Add(startStopButton);
        }

        #endregion

        #region User Interface
        /// <summary>
        /// Click handler for the start/stop button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void startStopButton_Click(object sender, EventArgs e)
        {
            // If the accelerometer is null, it is initialized and started
            if (accelerometer == null)
            {
                // Instantiate the accelerometer sensor object
                accelerometer = new Accelerometer();

                // Add an event handler for the ReadingChanged event.
                accelerometer.ReadingChanged += new EventHandler<AccelerometerReadingEventArgs>(accelerometer_ReadingChanged);

                // The Start method could throw and exception, so use a try block
                try
                {
                    statusTextBlock.Text = "starting accelerometer";
                    accelerometer.Start();
                }
                catch (AccelerometerFailedException exception)
                {
                    statusTextBlock.Text = "error starting accelerometer";
                }
            }
            else
            {
                // if the accelerometer is not null, call Stop
                try
                {
                    accelerometer.Stop();
                    accelerometer = null;
                    statusTextBlock.Text = "accelerometer stopped";
                }
                catch (AccelerometerFailedException exception)
                {
                    statusTextBlock.Text = "error stopping accelerometer";
                }

            }
        }
        #endregion

        #region Accelerometer Event Handling
        /// <summary>
        /// The event handler for the accelerometer ReadingChanged event.
        /// BeginInvoke is used to pass this event args object to the UI thread.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void accelerometer_ReadingChanged(object sender, AccelerometerReadingEventArgs e)
        {
            Deployment.Current.Dispatcher.BeginInvoke(() => MyReadingChanged(e));
        }
        /// <summary>
        /// Method for handling the ReadingChanged event on the UI thread.
        /// This sample just displays the reading value.
        /// </summary>
        /// <param name="e"></param>
        void MyReadingChanged(AccelerometerReadingEventArgs e)
        {
            if (accelerometer != null)
            {
                statusTextBlock.Text = accelerometer.State.ToString();
                XTextBlock.Text = e.X.ToString("0.00");
                YTextBlock.Text = e.Y.ToString("0.00");
                ZTextBlock.Text = e.Z.ToString("0.00");
            }
        }

        #endregion
    }
}