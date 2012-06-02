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

namespace ApplicationBarSample
{
    public partial class MainPage : PhoneApplicationPage
    {
        #region Initialization

        /// <summary>
        /// Constructor for the PhoneApplicationPage
        /// The ApplicationBar is initialized. Icon buttons and menu items are added
        /// to the ApplicationBar and event handlers are set.
        /// </summary>
        public MainPage()
        {
            InitializeComponent();

            this.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;

            ApplicationBar = new ApplicationBar();
            ApplicationBar.IsMenuEnabled = true;
            ApplicationBar.IsVisible = true;
            ApplicationBar.Opacity = 1.0;


            ApplicationBarIconButton hide = new ApplicationBarIconButton(new Uri("/Images/expand.png", UriKind.Relative));
            hide.Text = "hide";
            hide.Click += new EventHandler(hide_Click);

            ApplicationBarIconButton opacity = new ApplicationBarIconButton(new Uri("/Images/opacity.png", UriKind.Relative));
            opacity.Text = "opacity";
            opacity.Click += new EventHandler(opacity_Click);

            ApplicationBarIconButton enabled = new ApplicationBarIconButton(new Uri("/Images/menuenabled.png", UriKind.Relative));
            enabled.Text = "enabled";
            enabled.Click += new EventHandler(enabled_Click);

            ApplicationBar.Buttons.Add(hide);
            ApplicationBar.Buttons.Add(opacity);
            ApplicationBar.Buttons.Add(enabled);

            ApplicationBarMenuItem foregroundItem = new ApplicationBarMenuItem("use foreground color");
            foregroundItem.Click += new EventHandler(foregroundItem_Click);

            ApplicationBarMenuItem accentItem = new ApplicationBarMenuItem("use accent color");
            accentItem.Click += new EventHandler(accentItem_Click);

            ApplicationBar.MenuItems.Add(foregroundItem);
            ApplicationBar.MenuItems.Add(accentItem);



            UpdateText();

        }

        #endregion

        #region User Iterface

        /// <summary>
        /// Click handler for accent color menu item.
        /// Changes the colored UI elements to the built-in PhoneAccentColor
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        void accentItem_Click(object sender, EventArgs e)
        {
            UpdateColor((Color)Resources["PhoneAccentColor"]);
        }

        /// <summary>
        /// Click handler for accent color menu item.
        /// Changes the colored UI elements to the built-in PhoneForegroundColor
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        void foregroundItem_Click(object sender, EventArgs e)
        {
            UpdateColor((Color)Resources["PhoneForegroundColor"]);
        }

        /// <summary>
        /// Click handler for opacity icon button.
        /// Sets the opacity value of the ApplicationBar to 0, 1, or .5
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        void opacity_Click(object sender, EventArgs e)
        {
            if (ApplicationBar.Opacity < .01)
            {
                ApplicationBar.Opacity = 1;
            }
            else if (ApplicationBar.Opacity > .49 && ApplicationBar.Opacity < .51)
            {
                ApplicationBar.Opacity = 0;
            }
            else
            {
                ApplicationBar.Opacity = .5;
            }
            UpdateText();
        }

        /// <summary>
        /// Click handler for hide icon button.
        /// Changes the Visible property of the ApplicationBar to false
        /// And makes the "Show Application Bar" button visible
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        void hide_Click(object sender, EventArgs e)
        {
            ApplicationBar.IsVisible = false;
            showButton.Visibility = Visibility.Visible;
            UpdateText();
        }

        /// <summary>
        /// Click handler for menu enable icon button.
        /// Changes the IsMenuEnabled property of the ApplicationBar
        /// When IsMenuEnabled is false, the menu will not pop up
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        void enabled_Click(object sender, EventArgs e)
        {
            ApplicationBar.IsMenuEnabled = !ApplicationBar.IsMenuEnabled;
            UpdateText();
        }

        /// <summary>
        /// Click handler for show button.
        /// Sets the Visible property of the Application Bar to true
        /// </summary>
        /// <param name="sender">The control that raised the click event.</param>
        /// <param name="e">An EventArgs object containing event data.</param>
        private void showButton_Click(object sender, RoutedEventArgs e)
        {
            ApplicationBar.IsVisible = true;
            showButton.Visibility = Visibility.Collapsed;
            UpdateText();
        }

        /// <summary>
        /// Updates the TextBlock objects to reflect the current state
        /// of the ApplicationBar
        /// </summary>
        void UpdateText()
        {
            VisibleTextBlock.Text = ApplicationBar.IsVisible ? "yes" : "no";
            OpacityTextBlock.Text = ApplicationBar.Opacity.ToString("0.0");
            MenuEnabledTextBlock.Text = ApplicationBar.IsMenuEnabled ? "yes" : "no";
        }

        /// <summary>
        /// Helper method for changing the color of the UI
        /// </summary>
        /// <param name="c">The new color for the UI elements</param>
        void UpdateColor(Color c)
        {
            SolidColorBrush brush = new SolidColorBrush(c);
            VisibleTextBlock.Foreground = brush;
            OpacityTextBlock.Foreground = brush;
            MenuEnabledTextBlock.Foreground = brush;

            ((LinearGradientBrush)Resources["Gradient"]).GradientStops[1].Color = c;
        }

        #endregion
    }
}