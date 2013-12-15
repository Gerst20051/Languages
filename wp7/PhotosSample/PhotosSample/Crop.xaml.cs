
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
using Microsoft.Phone;
using Microsoft.Phone.Shell;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows.Controls.Primitives;
using Microsoft.Xna.Framework.Media;

namespace PhotosSample
{
    public partial class Crop : PhoneApplicationPage
    {

        //Variables for the application bar buttons
        ApplicationBarIconButton btnCrop;
        ApplicationBarIconButton btnAccept;
        ApplicationBarIconButton btnReject;
        ApplicationBarIconButton btnHelp;


        //Variable for the help popup
        Popup help = new Popup();

        //Variables for the crop feature
        Point p1, p2;
        bool cropping = false;


        public Crop()
        {
            InitializeComponent();

            textStatus.Text = "Select the cropping region with your finger." + " Once completed, tap the crop button to crop the image.";


            //Sets the source to the Image control on the crop page to the WriteableBitmap object created previously.
            DisplayedImageElement.Source = App.CapturedImage;


            //Create event handlers for cropping selection on the picture.
            DisplayedImageElement.MouseLeftButtonDown += new MouseButtonEventHandler(CropImage_MouseLeftButtonDown);
            DisplayedImageElement.MouseLeftButtonUp += new MouseButtonEventHandler(CropImage_MouseLeftButtonUp);
            DisplayedImageElement.MouseMove += new MouseEventHandler(CropImage_MouseMove);


            //Used for rendering the cropping rectangle on the image.
            CompositionTarget.Rendering += new EventHandler(CompositionTarget_Rendering);





            //Creating an application bar and then setting visibility and menu properties.
            ApplicationBar = new ApplicationBar();
            ApplicationBar.IsVisible = true;
            ApplicationBar.IsMenuEnabled = true;

            //This code creates the application bar icon buttons.
            btnCrop = new ApplicationBarIconButton(new Uri("/Icons/appbar.edit.rest.png", UriKind.Relative));
            btnAccept = new ApplicationBarIconButton(new Uri("/Icons/appbar.check.rest.png", UriKind.Relative));
            btnReject = new ApplicationBarIconButton(new Uri("/Icons/appbar.cancel.rest.png", UriKind.Relative));
            btnHelp = new ApplicationBarIconButton(new Uri("/Icons/appbar.questionmark.rest.png", UriKind.Relative));

            //Labels for the application bar buttons.
            btnCrop.Text = "Crop";
            btnAccept.Text = "Accept";
            btnReject.Text = "Reject";
            btnHelp.Text = "Help";

            //This code adds buttons to application bar.
            ApplicationBar.Buttons.Add(btnCrop);
            ApplicationBar.Buttons.Add(btnAccept);
            ApplicationBar.Buttons.Add(btnReject);
            ApplicationBar.Buttons.Add(btnHelp);



            //This code will create event handlers for buttons.
            btnCrop.Click += new EventHandler(btnCrop_Click);
            btnAccept.Click += new EventHandler(btnAccept_Click);
            btnReject.Click += new EventHandler(btnReject_Click);
            btnHelp.Click += new EventHandler(btnHelp_Click);

            //Disable buttons so user cannot click until appropriate time.
            btnCrop.IsEnabled = false;
            btnAccept.IsEnabled = false;
            btnReject.IsEnabled = false;

            //Begin storyboard for rectangle color effect.
            Rectangle.Begin();
        }


        void btnHelp_Click(object sender, EventArgs e)
        {
            // Create a popup/message box for help and add content to the popup


            //Stack panel with a black background
            StackPanel panelHelp = new StackPanel();
            panelHelp.Background = new SolidColorBrush(Colors.Black);
            panelHelp.Width = 400;
            panelHelp.Height = 550;

            //Create a white border
            Border border = new Border();
            border.BorderBrush = new SolidColorBrush(Colors.White);
            border.BorderThickness = new Thickness(7.0);

            //Create a close button to exit popup
            Button close = new Button();
            close.Content = "Close";
            close.Margin = new Thickness(5.0);
            close.Click += new RoutedEventHandler(close_Click);


            //Create helper text
            TextBlock textblockHelp = new TextBlock();
            textblockHelp.FontSize = 24;
            textblockHelp.Foreground = new SolidColorBrush(Colors.White);
            textblockHelp.TextWrapping = TextWrapping.Wrap;
            textblockHelp.Text = "Use your finger on the image to define a cropping region." + " Once the region is selected, as seen with a rectangle, tap the crop button to crop the image." + " You may choose to save this image in the media library by tapping the check button on the application bar, or reject the cropping and return to the original image with the cancel button (X).";
            textblockHelp.Margin = new Thickness(5.0);

            //Add controls to stack panel
            panelHelp.Children.Add(textblockHelp);
            panelHelp.Children.Add(close);
            border.Child = panelHelp;

            // Set the Child property of Popup to the border 
            // that contains a stackpanel, textblock and button.
            help.Child = border;

            // Set where the popup will show up on the screen.   
            help.VerticalOffset = 150;
            help.HorizontalOffset = 40;

            // Open the popup.
            help.IsOpen = true;

        }


        /// <summary>
        /// Click event handler for the close button on the help popup.
        /// This will create a popup/message box for help and add content to the popup.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void close_Click(object sender, RoutedEventArgs e)
        {
            help.IsOpen = false;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            if (cropping)
            {

                rect.SetValue(Canvas.LeftProperty, (p1.X < p2.X) ? p1.X : p2.X);
                rect.SetValue(Canvas.TopProperty, (p1.Y < p2.Y) ? p1.Y : p2.Y);
                rect.Width = (int)Math.Abs(p2.X - p1.X);
                rect.Height = (int)Math.Abs(p2.Y - p1.Y);
            }
        }


        /// <summary>
        /// Click event handler for mouse move.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void CropImage_MouseMove(object sender, MouseEventArgs e)
        {
            p2 = e.GetPosition(DisplayedImageElement);
        }

        /// <summary>
        /// Click event handler for mouse left button up
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void CropImage_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            p2 = e.GetPosition(DisplayedImageElement);
            cropping = false;


        }

        /// <summary>
        /// Click event handler for mouse left button down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        void CropImage_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            btnCrop.IsEnabled = true;
            p1 = e.GetPosition(DisplayedImageElement);
            p2 = p1;
            rect.Visibility = Visibility.Visible;
            cropping = true;
        }

        /// <summary>
        /// Click event handler for the reject button on the application bar.
        /// This will allow you to reject the cropped image and set back to the original image.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        void btnReject_Click(object sender, EventArgs e)
        {
            //Sets the cropped image back to the original image. For users that want to revert changes.
            DisplayedImageElement.Source = App.CapturedImage;

            //Buttons are disabled and user cannot proceed to use the below until they crop an image again.
            btnCrop.IsEnabled = false;
            btnAccept.IsEnabled = false;
            btnReject.IsEnabled = false;

            //Instructional Text
            textStatus.Text = "Select the cropping region with your finger." + " Once completed, tap the crop button to crop the image.";

        }

        /// <summary>
        /// Click event handler for the accept button on the application bar.
        /// Saves cropped image to isolated storage, then into
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        void btnAccept_Click(object sender, EventArgs e)
        {
            //Make progress bar visible for the event handler as there may be posible latency.
            progressBar2.Visibility = Visibility.Visible;

            //Create filename for JPEG in isolated storage
            String tempJPEG = "TempJPEG.jpg";

            //Create virtual store and file stream. Check for duplicate tempJPEG files.
            var myStore = IsolatedStorageFile.GetUserStoreForApplication();
            if (myStore.FileExists(tempJPEG))
            {
                myStore.DeleteFile(tempJPEG);
            }
            IsolatedStorageFileStream myFileStream = myStore.CreateFile(tempJPEG);



            //Encode the WriteableBitmap into JPEG stream and place into isolated storage.
            Extensions.SaveJpeg(App.CroppedImage, myFileStream, App.CroppedImage.PixelWidth, App.CroppedImage.PixelHeight, 0, 85);
            myFileStream.Close();

            //Create a new file stream.
            myFileStream = myStore.OpenFile(tempJPEG, FileMode.Open, FileAccess.Read);

            //Add the JPEG file to the photos library on the device.
            MediaLibrary library = new MediaLibrary();
            Picture pic = library.SavePicture("SavedPicture.jpg", myFileStream);
            myFileStream.Close();

            progressBar2.Visibility = Visibility.Collapsed;

            textStatus.Text = "Picture saved to photos library on the device.";






        }



        /// <summary>
        /// Click event handler for the crop button on the application bar.
        /// This code creates the new cropped writeable bitmap object.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void btnCrop_Click(object sender, EventArgs e)
        {


            // Get the size of the source image captured by the camera
            double originalImageWidth = App.CapturedImage.PixelWidth;
            double originalImageHeight = App.CapturedImage.PixelHeight;


            // Get the size of the image when it is displayed on the phone
            double displayedWidth = DisplayedImageElement.ActualWidth;
            double displayedHeight = DisplayedImageElement.ActualHeight;

            // Calculate the ratio of the original image to the displayed image
            double widthRatio = originalImageWidth / displayedWidth;
            double heightRatio = originalImageHeight / displayedHeight;

            // Create a new WriteableBitmap. The size of the bitmap is the size of the cropping rectangle
            // drawn by the user, multiplied by the image size ratio.
            App.CroppedImage = new WriteableBitmap((int)(widthRatio * Math.Abs(p2.X - p1.X)), (int)(heightRatio * Math.Abs(p2.Y - p1.Y)));


            // Calculate the offset of the cropped image. This is the distance, in pixels, to the top left corner
            // of the cropping rectangle, multiplied by the image size ratio.
            int xoffset = (int)(((p1.X < p2.X) ? p1.X : p2.X) * widthRatio);
            int yoffset = (int)(((p1.Y < p2.Y) ? p1.Y : p2.X) * heightRatio);

            // Copy the pixels from the targeted region of the source image into the target image, 
            // using the calculated offset
            for (int i = 0; i < App.CroppedImage.Pixels.Length; i++)
            {
                int x = (int)((i % App.CroppedImage.PixelWidth) + xoffset);
                int y = (int)((i / App.CroppedImage.PixelWidth) + yoffset);
                App.CroppedImage.Pixels[i] = App.CapturedImage.Pixels[y * App.CapturedImage.PixelWidth + x];
            }

            // Set the source of the image control to the new cropped bitmap
            DisplayedImageElement.Source = App.CroppedImage;
            rect.Visibility = Visibility.Collapsed;


            //Enable  accept and reject buttons to save or discard current cropped image.
            //Disable crop button until a new cropping region is selected.
            btnAccept.IsEnabled = true;
            btnReject.IsEnabled = true;
            btnCrop.IsEnabled = false;

            //Instructional text
            textStatus.Text = "Continue to crop image, accept, or reject.";




        }



    }
}