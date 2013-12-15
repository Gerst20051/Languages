<?php
class stack{
        var $memory=array();
        function push($value){
                $this->memory[]=$value;
        }
        function pop(){
                $count=$this->count();
                if ($count){
                        $value=$this->memory[$count-1];
                        unset($this->memory[$count-1]);
                        return $value;
                }
                else return false;
        }
        function count(){
                return count($this->memory);
        }
};
?>