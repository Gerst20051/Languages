<?php
class dog {

    var $name;
    var $age;
    var $owner;

    function dog($in_name="unnamed",$in_age="0",$in_owner="unknown") {
        $this->name = $in_name;
        $this->age = $in_age;
        $this->owner = $in_owner;
    }

    function getage() {
        return ($this->age * 365);
    }
    
    function getowner() {
        return ($this->owner);
    }
    
    function getname() {
        return ($this->name);
    }
}
?>

<body bgcolor=white>
<h1>Beware the dog</h1>
<?php
$ourfirstdog = new dog("Rover",12,"Lisa and Graham");
$dogdisc = serialize($ourfirstdog);
 
$pet = unserialize($dogdisc);
$old = $pet->getage();
$name = $pet->getname();
print "Our first dog is called $name and is $old days old<br>";
print $dogdisc;
?>