<?php
session_start();

include ("db.inc.php");
include ("login.inc.php");

$zip = new ZipArchive();
$filename = "new.zip";

if ($zip -> open($filename, ZIPARCHIVE::CREATE) !== true) {
exit("cannot open <$filename>\n");
}

$zip -> close();

/*
Properties

status
Status of the Zip Archive

statusSys
System status of the Zip Archive

numFiles
Number of files in archive

filename
File name in the file system

comment
Comment for the archive

bool addEmptyDir ( string $dirname )
bool addFile ( string $filename [, string $localname ] )
bool addFromString ( string $localname , string $contents )
bool close ( void )
bool deleteIndex ( int $index )
bool deleteName ( string $name )
bool extractTo ( string $destination [, mixed $entries ] )
string getArchiveComment ( void )
string getCommentIndex ( int $index [, int $flags ] )
string getCommentName ( string $name [, int $flags ] )
mixed getFromIndex ( int $index [, int $flags ] )
mixed getFromName ( string $name [, int $flags ] )
string getNameIndex ( int $index )
string getStatusString ( void )
resource getStream ( string $name )
mixed locateName ( string $name [, int $flags ] )
mixed open ( string $filename [, int $flags ] )
bool renameIndex ( int $index , string $newname )
bool renameName ( string $name , string $newname )
mixed setArchiveComment ( string $comment )
mixed setCommentIndex ( int $index , string $comment )
mixed setCommentName ( string $name , string $comment )
mixed statIndex ( int $index [, int $flags ] )
mixed statName ( name $name [, int $flags ] )
mixed unchangeAll ( void )
mixed unchangeArchive ( void )
mixed unchangeIndex ( int $index )
mixed unchangeName ( string $name )

ZipArchive::addEmptyDir — Add a new directory
ZipArchive::addFile — Adds a file to a ZIP archive from the given path
ZipArchive::addFromString — Add a file to a ZIP archive using its contents
ZipArchive::close — Close the active archive (opened or newly created)
ZipArchive::deleteIndex — delete an entry in the archive using its index
ZipArchive::deleteName — delete an entry in the archive using its name
ZipArchive::extractTo — Extract the archive contents
ZipArchive::getArchiveComment — Returns the Zip archive comment
ZipArchive::getCommentIndex — Returns the comment of an entry using the entry index
ZipArchive::getCommentName — Returns the comment of an entry using the entry name
ZipArchive::getFromIndex — Returns the entry contents using its index
ZipArchive::getFromName — Returns the entry contents using its name
ZipArchive::getNameIndex — Returns the name of an entry using its index
ZipArchive::getStatusString — Returns the status error message, system and/or zip messages
ZipArchive::getStream — Get a file handler to the entry defined by its name (read only).
ZipArchive::locateName — Returns the index of the entry in the archive
ZipArchive::open — Open a ZIP file archive
ZipArchive::renameIndex — Renames an entry defined by its index
ZipArchive::renameName — Renames an entry defined by its name
ZipArchive::setArchiveComment — Set the comment of a ZIP archive
ZipArchive::setCommentIndex — Set the comment of an entry defined by its index
ZipArchive::setCommentName — Set the comment of an entry defined by its name
ZipArchive::statIndex — Get the details of an entry defined by its index.
ZipArchive::statName — Get the details of an entry defined by its name.
ZipArchive::unchangeAll — Undo all changes done in the archive
ZipArchive::unchangeArchive — Revert all global changes done in the archive.
ZipArchive::unchangeIndex — Revert all changes done to an entry at the given index
ZipArchive::unchangeName — Revert all changes done to an entry with the given name.

zip_close — Close a ZIP file archive
zip_entry_close — Close a directory entry
zip_entry_compressedsize — Retrieve the compressed size of a directory entry
zip_entry_compressionmethod — Retrieve the compression method of a directory entry
zip_entry_filesize — Retrieve the actual file size of a directory entry
zip_entry_name — Retrieve the name of a directory entry
zip_entry_open — Open a directory entry for reading
zip_entry_read — Read from an open directory entry
zip_open — Open a ZIP file archive
zip_read — Read next entry in a ZIP file archive
*/
/*
$zip = new ZipArchive;
$zip -> open('teste.zip');
$zip -> extractTo('./');
$zip -> close();
echo "Ok!";
*/
/*
function archivebackup($archiveFile, $file, $errMsg) {
$ziph = new ZipArchive();

if (file_exists($archiveFile)) {
if ($ziph -> open($archiveFile, ZIPARCHIVE::CHECKCONS) !== true) {
$errMsg = "Unable to Open $archiveFile";
return 1;
}
} else {
if ($ziph -> open($archiveFile, ZIPARCHIVE::CM_PKWARE_IMPLODE) !== true) {
$errMsg = "Could not Create $archiveFile";
return 1;
}
}

if (!$ziph -> addFile($file)) {
$errMsg = "error archiving $file in $archiveFile";
return 2;
}

$ziph -> close();

return 0;
}
*/
/*
$zip = new ZipArchive();
$filename = "./test112.zip";

if ($zip -> open($filename, ZIPARCHIVE::CREATE) !== true) {
exit("cannot open <$filename>\n");
}

$zip -> addFromString("testfilephp.txt" . time(), "#1 This is a test string added as testfilephp.txt.\n");
$zip -> addFromString("testfilephp2.txt" . time(), "#2 This is a test string added as testfilephp2.txt.\n");
$zip -> addFile($thisdir . "/too.php","/testfromfile.php");

echo "numfiles: " . $zip->numFiles . "\n";
echo "status:" . $zip->status . "\n";

$zip -> close();
*/
/*
$za = new ZipArchive();
$za -> open('test_with_comment.zip');
print_r($za);
var_dump($za);

echo "numFiles: " . $za -> numFiles . "\n";
echo "status: " . $za -> status. "\n";
echo "statusSys: " . $za -> statusSys . "\n";
echo "filename: " . $za -> filename . "\n";
echo "comment: " . $za -> comment . "\n";

for ($i = 0; $i < $za -> numFiles; $i++) {
echo "index: $i\n";
print_r($za -> statIndex($i));
}
echo "numFile:" . $za -> numFiles . "\n";
*/
/*
$reader = new XMLReader();
$reader -> open('zip://' . dirname(__FILE__) . '/test.odt#meta.xml');
$odt_meta = array();

while ($reader -> read()) {
if ($reader -> nodeType == XMLREADER::ELEMENT) {
$elm = $reader -> name;
} else {
if ($reader -> nodeType == XMLREADER::END_ELEMENT && $reader -> name == 'office:meta') {
break;
}

if (!trim($reader -> value)) {
continue;
}

$odt_meta[$elm] = $reader -> value;
}
}

print_r($odt_meta);
*/
/*
$zip = zip_open("/tmp/test2.zip");

if ($zip) {
while ($zip_entry = zip_read($zip)) {
echo "Name: " . zip_entry_name($zip_entry) . "\n";
echo "Actual Filesize: " . zip_entry_filesize($zip_entry) . "\n";
echo "Compressed Size: " . zip_entry_compressedsize($zip_entry) . "\n";
echo "Compression Method: " . zip_entry_compressionmethod($zip_entry) . "\n";

if (zip_entry_open($zip, $zip_entry, "r")) {
echo "File Contents:\n";
$buf = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));
echo "$buf\n";

zip_entry_close($zip_entry);
}

echo "\n";
}

zip_close($zip);
}
*/
/*
// creates a compressed zip file
function create_zip($files = array(), $destination = '', $overwrite = false) {
//if the zip file already exists and overwrite is false, return false 
if (file_exists($destination) && !$overwrite) { return false; } 
//vars 
$valid_files = array();
//if files were passed in... 
if (is_array($files)) { 
//cycle through each file 
foreach($files as $file) { 
//make sure the file exists 
if (file_exists($file)) { 
$valid_files[] = $file;
}
}
}

//if we have good files... 
if (count($valid_files)) { 
//create the archive 
$zip = new ZipArchive();

if ($zip -> open($destination, $overwrite ? ZIPARCHIVE::OVERWRITE : ZIPARCHIVE::CREATE) !== true) { 
return false;
} 
//add the files 
foreach($valid_files as $file) { 
$zip -> addFile($file, $file);
}
//debug
//echo 'The zip archive contains ',$zip->numFiles,' files with a status of ',$zip->status; 
//close the zip — done! 
$zip -> close();
//check to make sure the file exists 
return file_exists($destination);
} else { 
return false;
}
}

// I added a fourth parameter to the function: $distill_subdirectories = true
// add the files
foreach($valid_files as $file) {
if ($distill_subdirectories) {
$zip -> addFile($file, basename($file));
} else {
$zip -> addFile($file, $file);
}
}

// problem
$zip -> addFile($file, $file);
// which will strip out the file path (if any) and leave you only the file name for the 2nd variable in addFile
$just_name = preg_replace("/(.*)\/?([^\/]+)/","$2", $file);
$zip -> addFile($file,$just_name);
*/
/*
Compressing an entire folder

function directoryToArray($directory, $recursive) {
$array_items = array();

if ($handle = opendir($directory)) {
while (false !== ($file = readdir($handle))) {
if ($file != "." && $file != ".." && $file != "Thumbs.db") {
if (is_dir($directory. "/" . $file)) {
if ($recursive) {
$array_items = array_merge($array_items, directoryToArray($directory. "/" . $file, $recursive));
}

$file = $directory . "/" . $file;
$array_items[] = preg_replace("/\/\//si", "/", $file);
} else {
$file = $directory . "/" . $file;
$array_items[] = preg_replace("/\/\//si", "/", $file);
}
}
}

closedir($handle);
}

return $array_items;
}

$files_to_zip = directoryToArray("myfolder", true);
$result = create_zip($files_to_zip,’my-archive.zip’);
*/
/*
// very short version of function for unzipping files with folders structure:

function unzip($file) {
$zip = zip_open(realpath(".") . "/" . $file);
if (!$zip) { return("Unable to proccess file '{$file}'"); }

$e = '';

while ($zip_entry = zip_read($zip)) {
$zdir = dirname(zip_entry_name($zip_entry));
$zname = zip_entry_name($zip_entry);

if (!zip_entry_open($zip,$zip_entry,"r")) { $e .= "Unable to proccess file '{$zname}'"; continue; }
if (!is_dir($zdir)) mkdirr($zdir,0777);

#print "{$zdir} | {$zname} \n";

$zip_fs = zip_entry_filesize($zip_entry);
if (empty($zip_fs)) continue;

$zz = zip_entry_read($zip_entry,$zip_fs);
$z = fopen($zname,"w");
fwrite($z,$zz);
fclose($z);
zip_entry_close($zip_entry);

}
zip_close($zip);

return($e);
}

function mkdirr($pn, $mode = null) {
if (is_dir($pn) || empty($pn)) return true;
$pn = str_replace(array('/', ''), DIRECTORY_SEPARATOR, $pn);

if (is_file($pn)) { trigger_error('mkdirr() File exists', E_USER_WARNING); return false; }

$next_pathname = substr($pn, 0, strrpos($pn, DIRECTORY_SEPARATOR));
if (mkdirr($next_pathname, $mode)) { if (!file_exists($pn)) { return mkdir($pn,$mode); } }
return false;
}

unzip("test.zip");
*/
/*
class MyZipArchive extends ZipArchive {

// Adds a directory recursively.
// @param string $filename The path to the file to add.
// @param string $localname Local name inside ZIP archive.
 
public function addDir($filename, $localname) {
$this -> addEmptyDir($localname);
$iter = new RecursiveDirectoryIterator($filename);

foreach ($iter as $fileinfo) {
if (! $fileinfo -> isFile() && !$fileinfo -> isDir()) {
continue;
}

$method = $fileinfo -> isFile() ? 'addFile' : 'addDir';
$this -> $method($fileinfo -> getPathname(), $localname . '/' . $fileinfo -> getFilename());
}
}
}
*/
/*
function unzip($zipfile) {
$zip = zip_open($zipfile);

while ($zip_entry = zip_read($zip)) {
zip_entry_open($zip, $zip_entry);

if (substr(zip_entry_name($zip_entry), -1) == '/') {
$zdir = substr(zip_entry_name($zip_entry), 0, -1);

if (file_exists($zdir)) {
trigger_error('Directory "<b>' . $zdir . '</b>" exists', E_USER_ERROR);
return false;
}

mkdir($zdir);
} else {
$name = zip_entry_name($zip_entry);

if (file_exists($name)) {
trigger_error('File "<b>' . $name . '</b>" exists', E_USER_ERROR);
return false;
}

$fopen = fopen($name, "w");
fwrite($fopen, zip_entry_read($zip_entry, zip_entry_filesize($zip_entry)), zip_entry_filesize($zip_entry));
}

zip_entry_close($zip_entry);
}

zip_close($zip);
return true;
}
*/
/*
function unpackZip($dir,$file) {
if ($zip = zip_open($dir . $file . ".zip")) {
if ($zip) {
mkdir($dir.$file);

while ($zip_entry = zip_read($zip)) {
if (zip_entry_open($zip,$zip_entry,"r")) {
$buf = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));
$dir_name = dirname(zip_entry_name($zip_entry));

if ($dir_name != ".") {
$dir_op = $dir . $file . "/";

foreach (explode("/", $dir_name) as $k) {
$dir_op = $dir_op . $k;

if (is_file($dir_op)) unlink($dir_op);
if (!is_dir($dir_op)) mkdir($dir_op);
$dir_op = $dir_op . "/" ;
}
}

$fp = fopen($dir . $file . "/" . zip_entry_name($zip_entry),"w");
fwrite($fp,$buf);
fclose($fp);

zip_entry_close($zip_entry);
} else return false;
}

zip_close($zip);
}
} else return false;

return true;
}
*/
/*
function unzip($zip_file, $src_dir, $extract_dir) { 
copy($src_dir . "/" . $zip_file, $extract_dir . "/" . $zip_file); 
chdir($extract_dir); 
shell_exec("unzip $zip_file"); 
}
*/

mysql_close($db);
?>