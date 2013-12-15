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
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace SettingsSample
{
    public partial class SettingsWithConfirmation : PhoneApplicationPage
    {

        private AppSettings settings = new AppSettings();


        public SettingsWithConfirmation()
        {

            InitializeComponent();

            // Add an Application Bar that has a 'done' confirmation button and 
            // a 'cancel' button
            ApplicationBar = new ApplicationBar();
            ApplicationBar.IsMenuEnabled = true;
            ApplicationBar.IsVisible = true;
            ApplicationBar.Opacity = 1.0;

            ApplicationBarIconButton doneButton = new ApplicationBarIconButton(new Uri("/Images/appbar.check.rest.png", UriKind.Relative));
            doneButton.Text = "done";
            doneButton.Click += new EventHandler(doneButton_Click);

            ApplicationBarIconButton cancelButton = new ApplicationBarIconButton(new Uri("/Images/appbar.cancel.rest.png", UriKind.Relative));
            cancelButton.Text = "cancel";
            cancelButton.Click += new EventHandler(cancelButton_Click);

            ApplicationBar.Buttons.Add(doneButton);
            ApplicationBar.Buttons.Add(cancelButton);

            // Copy the current settings into the text boxes as the new values
            // entered in will not be saved until the user clicks the 'done' button.
            textBoxUsername.Text = settings.UsernameSetting;
            passwordBoxPassword.Password = settings.PasswordSetting;

        }

        void doneButton_Click(object sender, EventArgs e)
        {
            settings.UsernameSetting = textBoxUsername.Text;
            settings.PasswordSetting = passwordBoxPassword.Password;
            NavigationService.GoBack();
        }

        void cancelButton_Click(object sender, EventArgs e)
        {
            NavigationService.GoBack();
        }


    }
}