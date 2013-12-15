<?php 
class A { 
    public $a = 1; 
    public $b = 2; 
    public $collection = array(); 

    function  __construct(){ 
        for ( $i=3; $i-->0;){ 
            array_push($this->collection, new B); 
        } 
    } 
} 

class B { 
    public $a = 1; 
    public $b = 2; 
} 

echo json_encode(new A); 

/*
{ 
    "a":1, 
    "b":2, 
    "collection":[{ 
        "a":1, 
        "b":2 
    },{ 
        "a":1, 
        "b":2 
    },{ 
        "a":1, 
        "b":2 
    }] 
}
*/
?>