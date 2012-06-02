
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
using System.Globalization;
using System.Threading;
using Microsoft.Phone.Shell;

namespace GlobalizationSample
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            SupportedOrientations = SupportedPageOrientation.Portrait | SupportedPageOrientation.Landscape;

            BuildApplicationBar();

        }
        private void LocList_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
        {
            // Set the current culture according to the selected locale and display information such as
            // date, time, currency, etc in the appropriate format.

            string nl;
            string cul;

            nl = locList.SelectedIndex.ToString();


            switch (nl)
            {
                
                case "0":
                    cul = "fr-FR";
                    break;
                case "1":
                    cul = "de-DE";
                    break;
                case "2":
                    cul = "it-IT";
                    break;
                case "3":
                    cul = "es-ES";
                    break;
                case "4":
                    cul = "en-GB";
                    break;
                case "5":
                    cul = "en-US";
                    break;
                case "6":
                    cul = "da-DK";
                    break;
                default:
                    cul = "en-US";
                    break;
            }


            // set this thread's current culture to the culture associated with the selected locale
            CultureInfo newCulture = new CultureInfo(cul);
            Thread.CurrentThread.CurrentCulture = newCulture;

            CultureInfo cc, cuic;
            cc = Thread.CurrentThread.CurrentCulture;
            cuic = Thread.CurrentThread.CurrentUICulture;

            // display the culture name in the language of the selected locale
            regionalFrmt.Text = cc.NativeName;

            // display the culture name in the localized language
            displayLang.Text = cuic.DisplayName;

            // display the date formats (long and short form) for the  current culuture  
            DateTime curDate = DateTime.Now;
            longDate.Text = cc.DateTimeFormat.LongDatePattern.ToString() + " " + curDate.ToString("D");
            shortDate.Text = cc.DateTimeFormat.ShortDatePattern.ToString() + "   " + curDate.ToString("d");

            // display the time format (long form) for the current culture
            longTime.Text = cc.DateTimeFormat.LongTimePattern + "   " + curDate.ToString("T");

            // display the currency format and currency symbol for the current culture
            Int64 money = 123456789;
            currencyFrmt.Text = money.ToString("C");

            // create a list of strings and use Array.Sort to take advantage of the current culture
            string[] stringArray = new string[] { "Æble", "Apple", "Znefol" };
            Array.Sort(stringArray);
            sortedStrings.Text = "";
            foreach (string s in stringArray)
            {
                sortedStrings.Text += s + "\n";
            }

        }
        // Helper function to build a localized ApplicationBar
        private void BuildApplicationBar()
        {
            // Set the page's ApplicationBar to a new instance of ApplicationBar
            ApplicationBar = new ApplicationBar();

            // Create a new button and set the text value to the localized string from AppResources
            ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("ApplicationIcon.png",UriKind.Relative));
            appBarButton.Text = AppResources.ButtonText;
            ApplicationBar.Buttons.Add(appBarButton);

            // Create a new menu item with the localized string from AppResources
            ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.MenuItemText);
            ApplicationBar.MenuItems.Add(appBarMenuItem);

        }
    }
}