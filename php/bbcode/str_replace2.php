<?php 
//variables from the form 
$title = $_POST['title']; 
$content = $_POST['content']; 
$date = date("F j Y"); 
$user = $_SESSION['username']; 

// Starts the BBcode.
$bbcode = array("[u]", "[u]", "[/u]", "[/u]", "[i]", "[i]", "[/i]", "[/i]", "[b]", "[b]", "[/b]", "[/b]", ":)", ":(" , ":D", ":p"); // separate with commas. 

$newbbcode = array("<u>", "<u>", "</u>", "</u>", "<i>", "<i>", "</i>", "</i>", "<b>", "<b>", "</b>", "</b>", "<img src=\"./smile.gif\">", "<img src=\"./frown.gif\">", "<img src=\"./big-smile.gif\">", "<img src=\"./tongue.gif\">"); 

$content =  str_replace($bbcode, $newbbcode, $content);

// this is something that I'm including to help you from getting hacked.

// to block more words just add them in quotes and seperate with commas.
$badWords = array("<?php", "<?PHP", "?>");

foreach($badWords AS $v) {
    // Replaces the words to nothing.
    $content = str_replace($v, "", $content);
    $title = str_replace($v, "", $content);
}

//check that user have filled in everything 
if(!$title && !$content) {
    $error = "<li>Sorry, you must fill in <strong>all</strong> fields.  Please go back and try again.</li>";
}else if(!$title) {
    $error = "<li>Sorry, you must fill in the field <b>Title</b>. Please go back and try again.</li>";
}else if(!$content) {
    $error = "<li>Sorry, you must fill in the field <b>Content</b>. Please go back and try again.</li>";
}

// This counts the errors and if there was more than one then echo out the error.
if(count($error) > 0) {
    echo $error;
// If there wasn't an error than insert into the table.
}else {
//perform query, inseting user submitted data 
mysql_query("INSERT INTO news (title, content, date, user) VALUES ('$title', '$content', '$date', '$user')") or die(mysql_error()); 
//message if everything went well. 
echo("<center><br><br><br>Thanks for posting.  Click <a href=\"/lodge/mainpage.php\">here</a> to return to the member's page.</font></center>");    
}

ob_end_flush(); 
?> 