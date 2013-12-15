=== Facebook Like Button Plugin ===
Contributors: martinjonsson
Donate link: https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=martin%2ejonsson%40gmail%2ecom&lc=SE&item_name=Martin%20Jonsson&item_number=facebook%2dlike%2dbutton%2dplugin&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted
Tags: facebook, like, socialwidget, social, button
Requires at least: 2.8
Tested up to: 2.9.2
Stable tag: 1.3.1

The new Facebook Like Button. Add and customize the behavior and appearance to you likings.

== Description ==

Add the new Facebook Like Button to your wordpress blog with ease.
Fully configurable and customizable to your likings.
Configure where you want it to be visible or you can include it on single post with shortcode.

If you don't know what the Facebook Like Button is, here is how Facebook describes it:
The Like button enables users to make connections to your pages and share content back to their friends on Facebook with one click. Since the content is hosted by Facebook, the button can display personalized content whether or not the user has logged into your site. For logged-in Facebook users, the button is personalized to highlight friends who have also liked the page.

== Installation ==

1. Download the installation zip file and unzip leaving the directory structure in tact.
2. Upload the newly unzipped 'facebook-like-button-plugin' folder to the `/wp-content/plugins/` folder
3. Activate the Facebook Like Button plugin from the 'Plugins' menu.
4. Choose customization on the plugin menu "Facebook like button"

== Shortcode syntax ==

= Example =

[fb_like_button_standard] - shows the button in standard layout.

[fb_like_button_count] - shows the button in button_count layout.

= Example with optional parameters =

[fb_like_button_count show_faces="1" action="like" font="arial" iframe_style="margin:10px;"]

= Optional parameters: =

* show_faces = boolean (1 / 0) 
* width = numeric, width in pixels
* height = numeric, height in pixels
* action = (like / recommend), Verb to display
* font = (arial / lucida grande / segoe ui / tahoma / trebuchet ms / verdana)
* colorscheme = (light / dark)
* iframe_style = custom css style for iframe/xfbml


If no optional parameters are used it will use the values from your saved configuration.

== Screenshots ==

1. Options

== Changelog ==

= 1.3.1 =
* Added option to show/hide button on Archive pages.

= 1.3 =
* Added ability to choose XFBML instead of iframe.
* Added headers for FB profile/application id.
* Added thumbnail meta if a post has a thumbnail.
* Moved the setting link to "Settings" group.
* Added link in plugins listing to settings.
* Added namespaces for opengraph and XFBML in html element.
* Added language setting for XFBML output

= 1.2 =
* Added shortcode [fb_like_button_standard] and [fb_like_button_count]
* Added option to _not_ show on single posts so you can add it manually on pages and posts with shortcode
* Added new meta tag og:site_name
* Added ability to suppress the button on post with custom_field "facebook_like_button" with value "suppress"
 
= 1.1 =
* Added param to set position top or bottom in relation to the content.
* Added param to set extra css style to the iframe.
* Removed wrapper paragraph, use the extra css style if you want to change margin/padding.
 
= 1.0 =
* First release

== Upgrade Notice ==

= 1.3.1 =
* Added option to show/hide button on Archive pages.

XFBML output gives the user a comment dialog when clicking the like button.

* Added ability to choose XFBML instead of iframe.
* Added headers for FB profile/application id.
* Added thumbnail meta if a post has a thumbnail.
* Moved the setting link to "Settings" group.
* Added link in plugins listing to settings.
* Added namespaces for opengraph and XFBML in html element.
* Added language setting for XFBML output

= 1.3 =
XFBML output gives the user a comment dialog when clicking the like button.

* Added ability to choose XFBML instead of iframe.
* Added headers for FB profile/application id.
* Added thumbnail meta if a post has a thumbnail.
* Moved the setting link to "Settings" group.
* Added link in plugins listing to settings.
* Added namespaces for opengraph and XFBML in html element.
* Added language setting for XFBML output

= 1.2 =
* Added shortcode [fb_like_button_standard] and [fb_like_button_count]
* Added option to _not_ show on single posts so you can add it manually on pages and posts with shortcode
* Added new meta tag og:site_name
* Added ability to suppress the button on post with custom_field "facebook_like_button" with value "suppress"

= 1.1 =
* Added param to set position top or bottom in relation to the content.
* Added param to set extra css style to the iframe.
* Removed wrapper paragraph, use the extra css style if you want to change margin/padding.


== Frequently Asked Questions ==

= How do I turn off the button on a single post? =

Give the post a [custom field](http://codex.wordpress.org/Custom_Fields) with the key "facebook_like_button", and the value "suppress". You can do this for as many posts as you want. If you remove the custom field, the button will re-appear on that post.

= Why doesn't the comment form show when I use XFBML? =

Make sure you have added a Facebook App ID.
If you don't have one you can create one [here](http://www.facebook.com/developers/apps.php)
Make sure the "Connect URL" in the Facebook Application Connect Settings has the same url as your wordpress site.

For any questions, comments or suggestions regarding the plugin please visit http://www.martinj.net