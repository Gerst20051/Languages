<?php
/*
Another way for pre-5.2.0 PHP users is using rawurlencode() in PHP to encode a string and decodeURIComponent() in javascript to decode it. I have written following class to handle PHP arrays and convert them to javascript format. It uses object notation for associative arrays and arrays for the other. Nesting is supported. True, false, integers, floats and null values are presented in respective javascript syntax.
Use: convert an array in PHP using this class, load it into the browser using ajax and then decode the strings in the resulting object using javascript function decodeData (below).
All of the example results have passed json validator so it shoud be allright. Feel free to use this.
PHP CLASS - encoding arrays
=======
*/

class custom_json {

    /**
     * Convert array to javascript object/array
     * @param array $array the array
     * @return string
     */
    public static function encode($array)
    {

        // determine type
        if(is_numeric(key($array))) {

            // indexed (list)
            $output = '[';
            for($i = 0, $last = (sizeof($array) - 1); isset($array[$i]); ++$i) {
                if(is_array($array[$i])) $output .= self::encode($array[$i]);
                else  $output .= self::_val($array[$i]);
                if($i !== $last) $output .= ',';
            }
            $output .= ']';

        } else {

            // associative (object)
            $output = '{';
            $last = sizeof($array) - 1;
            $i = 0;
            foreach($array as $key => $value) {
                $output .= '"'.$key.'":';
                if(is_array($value)) $output .= self::encode($value);
                else  $output .= self::_val($value);
                if($i !== $last) $output .= ',';
                ++$i;
            }
            $output .= '}';

        }

        // return
        return $output;

    }

    /**
     * [INTERNAL] Format value
     * @param mixed $val the value
     * @return string
     */
    private static function _val($val)
    {
        if(is_string($val)) return '"'.rawurlencode($val).'"';
        elseif(is_int($val)) return sprintf('%d', $val);
        elseif(is_float($val)) return sprintf('%F', $val);
        elseif(is_bool($val)) return ($val ? 'true' : 'false');
        else  return 'null';
    }

}

// prints ["apple","banana","blueberry"]
echo custom_json::encode(array('apple', 'banana', 'blueberry'));

// prints {"name":"orange","type":"fruit"}
echo custom_json::encode(array('name' => 'orange', 'type' => 'fruit'));

// prints: ** try it yourself, cannot post long lines here **
$big_test = array(
    array(
        'name' => array('John', 'Smith'),
        'age' => 27,
        'sex' => 0,
        'height' => 180.53,
        'is_human' => true,
        'string' => 'Hello',
    ),
    array(
        'name' => array('Green', 'Alien'),
        'age' => 642,
        'sex' => null,
        'height' => 92.21,
        'is_human' => false,
        'string' => '?????!', // test utf8 here
    )
);

echo custom_json::encode($big_test);

/*
JAVASCRIPT FUNCTION - decode rawurlencoded() strings
==================
function decodeData(data) {
        for(var item in data) {
            var type = typeof data[item];
            if(type === 'object') decodeData(data[item]);
            else if(type === 'string') data[item] = decodeURIComponent(data[item]);
        }
    }
*/
?>
?>