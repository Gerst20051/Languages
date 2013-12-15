<?php
/*
|-----------------
| Author:	Life.Object
| E-Mail:	life.object@gmail.com
| Website:	http://www.tutorialchip.com/
| Help:		http://www.tutorialchip.com/php-password-generator-class/
| Version:	1.2
| Released: December 02, 2010
| Updated:	December 23, 2010
|------------------
*/

class chip_password_generator {
	
	/*
	|---------------------------
	| Properties
	|---------------------------
	*/
	
	private $args = array(
						'length'				=>	8,
						'alpha_upper_include'	=>	TRUE,
						'alpha_lower_include'	=>	TRUE,						
						'number_include'		=>	TRUE,
						'symbol_include'		=>	TRUE,	
					);
	
	private $alpha_upper = array( "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" );
	private $alpha_lower = array( "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" );
	private $number = array( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 );
	private $symbol = array( "-", "_", "^", "~", "@", "&", "|", "=", "+", ";", "!", ",", "(", ")", "{", "}", "[", "]", ".", "?", "%", "*", "#" );
	private $input = 4;

	/*
	|---------------------------
	| Constructor
	|
	| @public
	| @param array $args
	|
	|---------------------------
	*/
	
	public function __construct( $args = array() ) {
		
		$this->set_args( $args );
						
	}
	
	/*
	|---------------------------
	| Print variable in readable format
	|
	| @public
	| @param string|array|object $var
	|
	|---------------------------
	*/
	
	public function chip_print( $var ) { 
		
		echo "<pre>";
    	print_r($var);
   	 	echo "</pre>";
	
	}
	
	/*
	|---------------------------
	| Update default arguments
	| It will update default array of class i.e $args
	|
	| @private
	| @param array $args - input arguments
	| @param array $defatuls - default arguments 
	| @return array
	|
	|---------------------------
	*/
	
	private function chip_parse_args( $args = array(), $defaults = array() ) { 
		return array_merge( $defaults, $args );	 
	}
	
	/*
	|---------------------------
	| Set default arguments
	| It will set default array of class i.e $args
	|
	| @private
	| @param array $args
	| @return 0
	|
	|---------------------------
	*/
	
	private function set_args( $args = array() ) { 
		
		$defaults = $this->get_args();
		$args = $this->chip_parse_args( $args, $defaults );
		$this->args = $args;	 
	}
	
	/*
	|---------------------------
	| Get default arguments
	| It will get default array of class i.e $args
	|
	| @public
	| @return array
	|
	|---------------------------
	*/
	
	public function get_args() { 
		return $this->args;	 
	}
	
	/*
	|---------------------------
	| Get Alpha Upper Array
	| It will get default array of $alpha_upper
	|
	| @private
	| @return array
	|
	|---------------------------
	*/
	
	private function get_alpha_upper() { 
		return $this->alpha_upper;	 
	}
	
	/*
	|---------------------------
	| Get Alpha Lower Array
	| It will get default array of $alpha_lower
	|
	| @private
	| @return array
	|
	|---------------------------
	*/
	
	private function get_alpha_lower() { 
		return $this->alpha_lower;	 
	}
	
	/*
	|---------------------------
	| Get Number Array
	| It will get default array of $number
	|
	| @private
	| @return array
	|
	|---------------------------
	*/
	
	private function get_number() { 
		return $this->number;	 
	}
	
	/*
	|---------------------------
	| Get Symbol Array
	| It will get default array of $symbol
	|
	| @private
	| @return array
	|
	|---------------------------
	*/
	
	private function get_symbol() { 
		return $this->symbol;	 
	}
	
	/*
	|---------------------------
	| Generate Password
	| It will generate password
	|
	| @private
	| @return array
	|
	|---------------------------
	*/
	
	private function set_password() { 
		
		/* Temporary Array(s) */
		$temp = array();
		$exec = array();
		
		/* Arguments */
		$args = $this->get_args();	 
		extract($args);
		
		/* Minimum Validation */		
		if( $length <= 0 ) {
			return 0;
		}
		
		/* Execution Array Logic */
		
		/* Alpha Upper */
		if( $alpha_upper_include == TRUE ) {
			$alpha_upper = $this->get_alpha_upper();
			$exec[] = 1;
		}
		
		/* Alpha Lower */
		if( $alpha_lower_include == TRUE ) {
			$alpha_lower = $this->get_alpha_lower();
			$exec[] = 2;
		}
		
		/* Number */
		if( $number_include == TRUE ) {
			$number = $this->get_number();
			$exec[] = 3;
		}
		
		/* Symbol */
		if( $symbol_include == TRUE ) {
			$symbol = $this->get_symbol();
			$exec[] = 4;
		}
		
		/* Unique and Random Loop */
		$exec_count = count( $exec ) - 1;
		$input_index = 0;
		//$this->chip_print( $exec );
		
		for ( $i = 1; $i <= $length; $i++ ) {
			
			switch( $exec[$input_index] ) {
				
				case 1:				
				shuffle( $alpha_upper );
				$temp[] = $alpha_upper[0];
				unset( $alpha_upper[0] );				
				break;
				
				case 2:				
				shuffle( $alpha_lower );
				$temp[] = $alpha_lower[0];
				unset( $alpha_lower[0] );				
				break;
				
				case 3:				
				shuffle( $number );
				$temp[] = $number[0];
				unset( $number[0] );				
				break;
				
				case 4:				
				shuffle( $symbol );
				$temp[] = $symbol[0];
				unset( $symbol[0] );				
				break;
				
			}
			
			if ( $input_index < $exec_count ) {
				$input_index++;
			} else {
				$input_index = 0;
			}
		
		} // for ( $i = 1; $i <= $length; $i++ )
		
		/* Shuffle */
		shuffle($temp);
		
		/* Make Password */		
		$password = implode( $temp );
		
		return $password;
		
	}	
	
	/*
	|---------------------------
	| Generate Password
	| It will generate password
	|
	| @public
	| @return array
	|
	|---------------------------
	*/
	
	public function get_password() { 		
		return $this->set_password();		
	}	
	

	/*
	|---------------------------
	| Destructor
	|---------------------------
	*/
	
	public function __destruct() {
	}
}
?>