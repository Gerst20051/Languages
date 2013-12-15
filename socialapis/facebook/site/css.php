<?php
session_start();
header("Content-Type: text/css");
?>
/* ---------------------------------------------------- */
/* ----------- >>>  Global Style Sheet  <<< ----------- */
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Site Name:    Social HnS
/* Site Creator: Andrew Gerst
/* Site Created: Tue, 07 Dec 2010 16:40:05 -0400
/* Last Updated: <?php echo date(r, filemtime('css.php')) . "\n";?>
<?php if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) {?>/* Current User: <?php echo $_SESSION['fullname'] . "\n";} ?>
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* ------------ >>>  Table of Contents  <<< ----------- */
/* ---------------------------------------------------- */
/* CSS Reset
/* CCS Base
/* Buttons
/* - SPN
/* - Shiny
/* Main
/* Transparency
/* Floating & Clearing
/* Drag / Resize
/* ---------------------------------------------------- */

/* Begin CSS Reset */

html {
background-color: #fff;
color: #000;
font-size: 100.01%;
}

body, div, dl, dt, dd, ul, ol, li, h1, h2, h3, h4, h5, h6, pre, code, form, fieldset, legend, input, textarea, p, blockquote, th, td {
margin: 0;
padding: 0;
}

table {
border-collapse: collapse;
border-spacing: 0;
}

fieldset, img {
border: 0;
}

address, caption, cite, code, dfn, em, strong, th, var {
font-style: normal;
font-weight: normal;
}

li {
list-style: none;
}

caption, th {
text-align: left;
}

h1, h2, h3, h4, h5, h6 {
font-size: 100%;
font-weight: normal;
}

q:before, q:after {
content: '';
}

abbr, acronym {
border: 0;
font-variant: normal;
}

sup {
vertical-align: text-top;
}

sub {
vertical-align: text-bottom;
}

input, textarea, select {
font-family: inherit;
font-size: inherit;
font-weight: inherit;
}

input, textarea, select {
*font-size: 100%;
}

legend {
color: #000;
}

/* End CSS Reset */
/* Begin CSS Base */

html, body {
font-family: tahoma, arial, verdana, sans-serif;
font-size: 1em;
height: 100%;
line-height: 1.4;
margin: 0 auto;
overflow: hidden;
padding: 0 auto;
width: 100%;
}

html {
background-color: #fff;
background-image: url(i/wallpapers/vista.jpg);
background-position: center center;
background-repeat: no-repeat;
color: #000;
user-select: text;
-khtml-user-select: text;
-moz-user-select: text;
-webkit-user-select: text;
}

body {
height: 100%;
overflow: hidden;
user-select: none;
-khtml-user-select: none;
-moz-user-select: none;
-webkit-user-select: none;
width: 100%;
}

h1 {
font-size: 138.5%;
}

h2 {
font-size: 123.1%;
}

h3 {
font-size: 108%;
}

h1, h2, h3 {
margin: 1em 0;
}

h1, h2, h3, h4, h5, h6, strong {
font-weight: bold;
}

abbr, acronym {
border-bottom: 1px dotted #000;
cursor: help;
}

em {
font-style: italic;
}

blockquote, ul, ol, dl {
margin: 1em;
}

ol, ul, dl {
margin-left: 2em;
}

ol li {
list-style: decimal outside;
}

ul li {
list-style: disc outside;
}

dl dd {
margin-left: 1em;
}

th, td {
border: 1px solid #000;
padding: .5em;
}

th {
font-weight: bold;
text-align: center;
}

caption {
margin-bottom: .5em;
text-align: center;
}

p, fieldset, table {
margin-bottom: 1em;
}

/*
pre {
background: #eee;
padding: 10px;
border: 2px solid #c94a29;
overflow: hidden;
margin: 0 0 15px 0;
width: 563px;
font-family: Courier, Monospace;
}
*/
/* End CSS Base */
/* Begin HTML 5 */

address, article, aside, footer, header, hgroup, nav, section, time {
display: block;
}

address {
margin: 0 0 0.2em 0;
}

time {
margin: 0 0 1.5em 0;
}

/*
article:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
aside:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
div:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
form:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
header:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
nav:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
section:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
ul:after { clear: both; content: "."; display: block; height: 0; visibility: hidden; }
*/

/* End HTML5 */
/* Begin Buttons */

.buttons a,
.buttons button {/* if no image in button add padding-left: 10px */
background-color: #f5f5f5;
border: 1px solid #dedede;
border-left: 1px solid #eee;
border-top: 1px solid #eee;
color: #565656;
cursor: pointer;
display: block;
float: left;
font-family: "lucida grande", tahoma, arial, verdana, sans-serif;
font-size: 10pt;
font-weight: bold;
line-height: 130%;
margin: 0 7px 0 0;
padding: 5px 10px 6px 7px;
text-decoration: none;
}

.buttons button {
overflow: visible;
padding: 4px 10px 3px 7px;
width: auto;
}

.buttons button[type] {
line-height: 17px;
padding: 5px 10px 5px 7px;
}

*:first-child+html button[type] {
padding: 4px 10px 3px 7px;
}

.buttons a img,
.buttons button img {
border: none;
height: 16px;
margin: 0 3px -3px 0 !important;
padding: 0;
width: 16px;
}

/* STANDARD */

button:hover,
.buttons a:hover {
background-color: #dff4ff;
border: 1px solid #c2e1ef;
color: #336699;
}

.buttons a:active {
background-color: #6299c5;
border: 1px solid #6299c5;
color: #fff;
}

/* POSITIVE */

button.positive,
.buttons a.positive {
color: #529214;
}

.buttons a.positive:hover,
button.positive:hover {
background-color: #e6efc2;
border: 1px solid #c6d880;
color: #529214;
}

.buttons a.positive:active {
background-color: #529214;
border: 1px solid #529214;
color: #fff;
}

/* NEGATIVE */

.buttons a.negative,
button.negative {
color: #d12f19;
}

.buttons a.negative:hover,
button.negative:hover {
background: #fbe3e4;
border: 1px solid #fbc2c4;
color: #d12f19;
}

.buttons a.negative:active {
background-color: #d12f19;
border: 1px solid #d12f19;
color: #fff;
}

/** Begin Shiny Buttons */

.shiny-button1,
.shiny-button2,
.shiny-button3 {
background-color: #666;
background-color: rgba(128,128,128,0.75);
background-image: -moz-linear-gradient(top, bottom, from(rgba(64,64,64,0.75)), to(rgba(192,192,192,0.9)));
background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(64,64,64,0.75)), to(rgba(192,192,192,0.9)));
border: 2px solid #999;
border-radius: 16px;
-khtml-border-radius: 16px;
-moz-border-radius: 16px;
-opera-border-radius: 16px;
-webkit-border-radius: 16px;
box-shadow: rgba(192,192,192,0.75) 0 8px 24px;
-khtml-box-shadow: rgba(192,192,192,0.75) 0 8px 24px;
-moz-box-shadow: rgba(192,192,192,0.75) 0 8px 24px;
-webkit-box-shadow: rgba(192,192,192,0.75) 0 8px 24px;
color: #fff;
cursor: pointer;
display: inline-block;
font-size: 1.5em;
font-weight: bold;
padding: 0.25em 0.5em 0.3em 0.5em;
position: relative;
text-align: center;
text-shadow: 1px 1px 3px rgba(0,0,0,0.5);
user-select: none;
-khtml-user-select: none;
-moz-user-select: none;
-webkit-user-select: none;
width: 8em;
}

.shiny-button1 span,
.shiny-button2 span,
.shiny-button3 span {
background-color: rgba(255,255,255,0.25);
background-image: -moz-linear-gradient(top, bottom, from(rgba(255,255,255,0.75)), to(rgba(255,255,255,0)));
background-image: -webkit-gradient(linear, 0% 0%, 0% 100%, from(rgba(255,255,255,0.75)), to(rgba(255,255,255,0)));
border-radius: 8px;
-khtml-border-radius: 8px;
-moz-border-radius: 8px;
-opera-border-radius: 8px;
-webkit-border-radius: 8px;
display: block;
height: 50%;
left: 3.5%;
position: absolute;
top: 0;
width: 94%;
}

.shiny-button1:hover { /* red */
background-color: rgba(255,0,0,0.75);
background-image: -moz-linear-gradient(top, bottom, from(rgba(128,64,64,0.75)), to(rgba(192,128,128,0.9)));
background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(128,64,64,0.75)), to(rgba(256,128,128,0.9)));
border-color: #aa7777;
box-shadow: rgba(256,128,128,0.5) 0 8px 24px;
-khtml-box-shadow: rgba(256,128,128,0.5) 0 8px 24px;
-moz-box-shadow: rgba(256,128,128,0.5) 0 8px 24px;
-webkit-box-shadow: rgba(256,128,128,0.5) 0 8px 24px;
}

.shiny-button2:hover { /* green */
background-color: rgba(0,128,0,0.75);
background-image: -moz-linear-gradient(top, bottom, from(rgba(64,128,64,0.75)), to(rgba(128,192,128,0.9)));
background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(64,128,64,0.75)), to(rgba(128,255,128,0.9)));
border-color: #77cc77;
box-shadow: rgba(128,256,128,0.6) 0 8px 24px;
-khtml-box-shadow: rgba(128,256,128,0.6) 0 8px 24px;
-moz-box-shadow: rgba(128,256,128,0.6) 0 8px 24px;
-webkit-box-shadow: rgba(128,256,128,0.6) 0 8px 24px;
}

.shiny-button3:hover { /* blue */
background-color: rgba(64,128,192,0.75);
background-image: -moz-linear-gradient(top, bottom, from(rgba(16,96,192,0.75)), to(rgba(96,192,255,0.9)));
background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(16,96,192,0.75)), to(rgba(96,192,255,0.9)));
border-color: #6699cc;
box-shadow: rgba(128,192,255,0.75) 0 8px 24px;
-khtml-box-shadow: rgba(128,192,255,0.75) 0 8px 24px;
-moz-box-shadow: rgba(128,192,255,0.75) 0 8px 24px;
-webkit-box-shadow: rgba(128,192,255,0.75) 0 8px 24px;
}

/* End Shiny Buttons **/
/* End Buttons */

/* ----------------------------------------------------- */
/* -------------- >>>  Global Layout  <<< -------------- */
/* ----------------------------------------------------- */

/* Begin Main */

div#blackout {
background-color: #000;
filter: alpha(opacity=60);
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=60)";
height: 100%;
opacity: 0.6;
-khtml-opacity: 0.6;
-moz-opacity: 0.6;
width: 100%;
}

div#whiteout {
background-color: #fff;
filter: alpha(opacity=60);
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=60)";
height: 100%;
opacity: 0.6;
-khtml-opacity: 0.6;
-moz-opacity: 0.6;
width: 100%;
}

div.breakwrap {
white-space: normal;
white-space: pre-wrap;
white-space: -pre-wrap;
white-space: -moz-pre-wrap;
white-space: -o-pre-wrap;
word-break: break-all;
word-wrap: break-word;
}

/* End Main */
/* Begin Transparency */

.transparent1 {
filter: alpha(opacity=10) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=10)" !important;
opacity: 0.1 !important;
-khtml-opacity: 0.1 !important;
-moz-opacity: 0.1 !important;
}

.transparent2 {
filter: alpha(opacity=20) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=20)" !important;
opacity: 0.2 !important;
-khtml-opacity: 0.2 !important;
-moz-opacity: 0.2 !important;
}

.transparent3 {
filter: alpha(opacity=30) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=30)" !important;
opacity: 0.3 !important;
-khtml-opacity: 0.3 !important;
-moz-opacity: 0.3 !important;
}

.transparent4 {
filter: alpha(opacity=40) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=40)" !important;
opacity: 0.4 !important;
-khtml-opacity: 0.4 !important;
-moz-opacity: 0.4 !important;
}

.transparent5 {
filter: alpha(opacity=50) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=50)" !important;
opacity: 0.5 !important;
-khtml-opacity: 0.5 !important;
-moz-opacity: 0.5 !important;
}

.transparent6 {
filter: alpha(opacity=60) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=60)" !important;
opacity: 0.6 !important;
-khtml-opacity: 0.6 !important;
-moz-opacity: 0.6 !important;
}

.transparent7 {
filter: alpha(opacity=70) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=70)" !important;
opacity: 0.7 !important;
-khtml-opacity: 0.7 !important;
-moz-opacity: 0.7 !important;
}

.transparent8 {
filter: alpha(opacity=80) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=80)" !important;
opacity: 0.8 !important;
-khtml-opacity: 0.8 !important;
-moz-opacity: 0.8 !important;
}

.transparent9 {
filter: alpha(opacity=90) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=90)" !important;
opacity: 0.9 !important;
-khtml-opacity: 0.9 !important;
-moz-opacity: 0.9 !important;
}

.transparent10 {
filter: alpha(opacity=100) !important;
-ms-filter: "progid:DXImageTransform.Microsoft.Alpha(opacity=100)" !important;
opacity: 1 !important;
-khtml-opacity: 1 !important;
-moz-opacity: 1 !important;
}

/* End Transparency */
/* Begin Floating & Clearing */

.clear {
clear: both !important;
}

.left {
float: left !important;
}

.right {
float: right !important;
}

/** Begin Clear Fix */

.clearfix:after {
clear: both;
content: " ";
display: block;
font-size: 0;
height: 0;
line-height: 0;
visibility: hidden;
width: 0;
}

.clearfix {
display: inline-table;
display: inline-block;
}

html[xmlns] .clearfix {
display: block;
}

/* Begin Hide From IE-Mac \*/

* html .clearfix {
height: 1%;
}

.clearfix {
display: block;
}

/* End Hide From IE-Mac */
/* End Clear Fix **/
/* End Floating & Clearing */
/* Begin Drag/Resize */

.drsElement {
position: absolute;
z-index: 1;
}

.drsMoveHandle {
background-color: transparent;
cursor: move;
height: 28px;
width: 100%;
}

.dragresize {
background-color: transparent;
border: 0;
font-size: 1px;
position: absolute;
}

.dragresize-tl {
cursor: nw-resize;
height: 6px;
left: 0;
top: 0;
width: 6px;
z-index: 101;
}

.dragresize-tm {
cursor: n-resize;
height: 6px;
left: 0;
top: 0;
width: 100%;
z-index: 100;
}

.dragresize-tr {
cursor: ne-resize;
height: 6px;
right: 0;
top: 0;
width: 6px;
z-index: 101;
}

.dragresize-ml {
cursor: w-resize;
top: 0;
left: 0;
height: 100%;
width: 6px;
z-index: 100;
}

.dragresize-mr {
cursor: e-resize;
height: 100%;
top: 0;
right: 0;
width: 6px;
z-index: 100;
}

.dragresize-bl {
bottom: 0;
cursor: sw-resize;
height: 6px;
left: 0;
width: 6px;
z-index: 101;
}

.dragresize-bm {
bottom: 0;
cursor: s-resize;
height: 6px;
left: 0;
width: 100%;
z-index: 100;
}

.dragresize-br {
bottom: 0;
cursor: se-resize;
height: 6px;
right: 0;
width: 6px;
z-index: 101;
}

/* End Drag/Resize */
/* Begin User Interface */
/* End User Interface */