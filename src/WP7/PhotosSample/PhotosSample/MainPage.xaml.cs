
/* 
    Copyright (c) 2010 Microsoft Corporation.  All rights reserved.
    Use of this sample source code is subject to the terms of the Microsoft license 
    agreement under which you licensed this sample source code and is provided AS-IS.
    If you did not accept the terms of the license agreement, you are not authorized 
    to use this sample source code.  For the terms of the license, please see the 
    license agreement between you and Microsoft.
*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using Microsoft.Phone;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows.Controls.Primitives;

namespace PhotosSample
{
    public partial class MainPage : PhoneApplicationPage
    {

        //This is a variable for the help popup.
        Popup help = new Popup();

        //The application bar buttons that are used.
        ApplicationBarIconButton btnCamera;
        ApplicationBarIconButton btnCrop;
        ApplicationBarIconButton btnHelp;

        //The camera chooser used to capture a picture.
        CameraCaptureTask ctask;



        // Constructor
        public MainPage()
        {
            InitializeComponent();


            SupportedOrientations = SupportedPageOrientation.Portrait | SupportedPageOrientation.Landscape;


            //Creates an application bar and then sets visibility and menu properties.
            ApplicationBar = new ApplicationBar();
            ApplicationBar.IsVisible = true;


            //This code creates the application bar icon buttons.
            btnCamera = new ApplicationBarIconButton(new Uri("/Icons/appbar.feature.camera.rest.png", UriKind.Relative));
            btnCrop = new ApplicationBarIconButton(new Uri("/Icons/appbar.edit.rest.png", UriKind.Relative));
            btnHelp = new ApplicationBarIconButton(new Uri("/Icons/appbar.questionmark.rest.png", UriKind.Relative));

            //Labels for the application bar buttons.
            btnCamera.Text = "Camera";
            btnCrop.Text = "Crop";
            btnHelp.Text = "Help";


            //This code will create event handlers for buttons.
            btnCamera.Click += new EventHandler(btnCamera_Click);
            btnCrop.Click += new EventHandler(btnCrop_Click);
            btnHelp.Click += new EventHandler(btnHelp_Click);



            //This code adds buttons to application bar.
            ApplicationBar.Buttons.Add(btnCamera);
            ApplicationBar.Buttons.Add(btnCrop);
            ApplicationBar.Buttons.Add(btnHelp);




            //Disable crop button until photo is taken.
            btnCrop.IsEnabled = false;

            textStatus.Text = "Tap the camera button to take a picture.";

            //Create new instance of CameraCaptureClass
            ctask = new CameraCaptureTask();


            //Create new event handler for capturing a photo
            ctask.Completed += new EventHandler<PhotoResult>(ctask_Completed);





        }



        /// <summary>
        /// Click event handler for the help button.
        ///This will create a popup/message box for help and add content to the popup.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void btnHelp_Click(object sender, EventArgs e)
        {





            //Stack panel with a black background.
            StackPanel panelHelp = new StackPanel();
            panelHelp.Background = new SolidColorBrush(Colors.Black);
            panelHelp.Width = 300;
            panelHelp.Height = 400;

            //Create a white border.
            Border border = new Border();
            border.BorderBrush = new SolidColorBrush(Colors.White);
            border.BorderThickness = new Thickness(7.0);

            //Create a close button to exit popup.
            Button close = new Button();
            close.Content = "Close";
            close.Margin = new Thickness(5.0);
            close.Click += new RoutedEventHandler(close_Click);


            //Create helper text
            TextBlock textblockHelp = new TextBlock();
            textblockHelp.FontSize = 24;
            textblockHelp.Foreground = new SolidColorBrush(Colors.White);
            textblockHelp.TextWrapping = TextWrapping.Wrap;
            textblockHelp.Text = "Tap the camera button image on the application bar to take a photo." + " Once the photo is taken and returned to this page, tap the crop button on the application bar to crop the image.";
            textblockHelp.Margin = new Thickness(5.0);

            //Add controls to stack panel
            panelHelp.Children.Add(textblockHelp);
            panelHelp.Children.Add(close);
            border.Child = panelHelp;

            // Set the Child property of Popup to the border 
            // that contains a stackpanel, textblock and button.
            help.Child = border;

            // Set where the popup will show up on the screen.   
            help.VerticalOffset = 200;
            help.HorizontalOffset = 85;

            // Open the popup.
            help.IsOpen = true;


        }


        /// <summary>
        /// Click event handler for the close button on the help popup.
        /// Closes the poupup.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void close_Click(object sender, RoutedEventArgs e)
        {
            help.IsOpen = false;
        }





        /// <summary>
        /// Click event handler for the crop button on the application bar.
        /// This will redirect the user to the cropping page.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void btnCrop_Click(object sender, EventArgs e)
        {
            //Error text for if user does not take a photo before choosing the crop button.
            if (App.CapturedImage == null)
            {
                textStatus.Text = "You must take a picture first.";

            }

            else
            {
                //If photo has been taken, crop button navigates to Crop.xaml page.
                NavigationService.Navigate(new Uri("/Crop.xaml", UriKind.Relative));

            }
        }








        /// <summary>
        /// Click event handler for the camera button.
        /// Opens the camera on the phone.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void btnCamera_Click(object sender, EventArgs e)
        {


            textStatus.Text = "";


            //Show the camera.
            ctask.Show();


            //Set progress bar to visible to show time between user snapshot and decoding
            //of image into a writeable bitmap object.
            progressBar1.Visibility = Visibility.Visible;



        }


        /// <summary>
        /// Event handler for retrieving the JPEG photo stream.
        /// Also to for decoding JPEG stream into a writeable bitmap and displaying.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void ctask_Completed(object sender, PhotoResult e)
        {

            if (e.TaskResult == TaskResult.OK && e.ChosenPhoto != null)
            {

                //Take JPEG stream and decode into a WriteableBitmap object
                App.CapturedImage = PictureDecoder.DecodeJpeg(e.ChosenPhoto);

                //Collapse visibility on the progress bar once writeable bitmap is visible.
                progressBar1.Visibility = Visibility.Collapsed;


                //Populate image control with WriteableBitmap object.
                MainImage.Source = App.CapturedImage;

                //Once writeable bitmap has been rendered, the crop button
                //is enabled.
                btnCrop.IsEnabled = true;




                textStatus.Text = "Tap the crop button to proceed";


            }

            else
            {

                textStatus.Text = "You decided not to take a picture.";
            }




        }




    }
}