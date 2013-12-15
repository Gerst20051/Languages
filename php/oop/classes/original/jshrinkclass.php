<?php
/*
  JShrink

  Copyright (c) 2009, Robert Hafner
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of conditions and the following
		disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
		following disclaimer in the documentation and/or other materials provided with the distribution.
	* Neither the name of the <ORGANIZATION> nor the names of its contributors may be used to endorse or promote
		products derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
 * JShrink
 *
 * Usage - JShrink::minify($js);
 * Usage - JShrink::minify($js, $options);
 * Usage - JShrink::minify($js, array('flaggedComments' => false));
 *
 * @version 0.2
 * @package JShrink
 * @author Robert Hafner <tedivm@tedivm.com>
 * @license http://www.opensource.org/licenses/bsd-license.php
 */
class JShrink
{
	protected $input;
	protected $index = 0;

	protected $a = '';
	protected $b = '';
	protected $c;

	protected $options;

	static protected $defaultOptions = array('flaggedComments' => true);

	static public function minify($js, $options = array())
	{
		try{
			$currentOptions = array_merge(self::$defaultOptions, $options);

			ob_start();
			$currentOptions = array_merge(self::$defaultOptions, $options);
			$me = new JShrink();
			$me->breakdownScript($js, $currentOptions);
			$output = ob_get_clean();
			return $output;

		}catch(Exception $e){
			ob_end_clean();
			throw $e;
		}
	}

	protected function breakdownScript($js, $currentOptions)
	{
		$this->options = $currentOptions;

		$js = str_replace("\r\n", "\n", $js);
		$this->input = str_replace("\r", "\n", $js);

		$this->a = $this->getReal();

		// the only time the length can be higher than 1 is if a conditional comment needs to be displayed
		// and the only time that can happen for $a is on the very first run
		while(strlen($this->a) > 1)
		{
			echo $this->a;
			$this->a = $this->getReal();
		}

		$this->b = $this->getReal();

		while($this->a !== false && !is_null($this->a) && $this->a !== '')
		{

			// now we give $b the same check for conditional comments we gave $a before we began looping
			if(strlen($this->b) > 1)
			{
				echo $this->a . $this->b;
				$this->a = $this->getReal();
				$this->b = $this->getReal();
				continue;
			}

			switch($this->a)
			{
				// new lines
				case "\n":
					// if the next line is something that can't stand alone preserver the newline
					if(strpos('(-+{[@', $this->b) !== false)
					{
						echo $this->a;
						$this->saveString();
						break;
					}

					// if its a space we move down to the string test below
					if($this->b === ' ')
						break;

					// otherwise we treat the newline like a space

				case ' ':
					if(self::isAlphaNumeric($this->b))
						echo $this->a;

					$this->saveString();
					break;

				default:
					switch($this->b)
					{
						case "\n":
							if(strpos('}])+-"\'', $this->a) !== false)
							{
								echo $this->a;
								$this->saveString();
								break;
							}else{
								if(self::isAlphaNumeric($this->a))
								{
									echo $this->a;
									$this->saveString();
								}
							}
							break;

						case ' ':
							if(!self::isAlphaNumeric($this->a))
								break;

						default:
							// check for some regex that breaks stuff
							if($this->a == '/' && ($this->b == '\'' || $this->b == '"'))
							{
								$this->saveRegex();
								continue;
							}

							echo $this->a;
							$this->saveString();
							break;
					}
			}

			// do reg check of doom
			$this->b = $this->getReal();

			if(($this->b == '/' && strpos('(,=:[!&|?', $this->a) !== false))
				$this->saveRegex();
		}
	}

	protected function getChar()
	{
		if(isset($this->c))
		{
			$char = $this->c;
			unset($this->c);
		}else{
			if(isset($this->input[$this->index]))
			{
				$char = $this->input[$this->index];
				$this->index++;
			}else{
				return false;
			}
		}

		if($char === "\n" || ord($char) >= 32)
			return $char;

		return ' ';
	}

	protected function getReal()
	{
		$startIndex = $this->index;
		$char = $this->getChar();

		if($char == '/')
		{
			$this->c = $this->getChar();

			if($this->c == '/')
			{
				$thirdCommentString = $this->input[$this->index];

				// kill rest of line
				$char = $this->getNext("\n");

				if($thirdCommentString == '@')
				{
					$endPoint = ($this->index) - $startIndex;
					unset($this->c);
					$char = "\n" . substr($this->input, $startIndex, $endPoint);// . "\n";
				}else{
					$char = $this->getChar();
					$char = $this->getChar();
				}

			}elseif($this->c == '*'){

				$this->getChar(); // current C
				$thirdCommentString = $this->getChar();

				if($thirdCommentString == '@')
				{
					// we're gonna back up a bit and and send the comment back, where the first
					// char will be echoed and the rest will be treated like a string
					$this->index = $this->index-2;
					return '/';

				}elseif($this->getNext('*/')){
				// kill everything up to the next */

					$this->getChar(); // get *
					$this->getChar(); // get /

					$char = $this->getChar(); // get next real charactor

					// if YUI-style comments are enabled we reinsert it into the stream
					if($this->options['flaggedComments'] && $thirdCommentString == '!')
					{
						$endPoint = ($this->index - 1) - $startIndex;
						echo "\n" . substr($this->input, $startIndex, $endPoint) . "\n";
					}

				}else{
					$char = false;
				}

				if($char === false)
					throw new JShrinkException('Stray comment. ' . $this->index);

				// if we're here c is part of the comment and therefore tossed
				if(isset($this->c))
					unset($this->c);
			}
		}
		return $char;
	}

	protected function getNext($string)
	{
		$pos = strpos($this->input, $string, $this->index);

		if($pos === false)
			return false;

		$this->index = $pos ;
		return $this->input[$this->index];
	}

	protected function saveString()
	{
		$this->a = $this->b;
		if($this->a == '\'' || $this->a == '"')
		{
			// save literal string
			$stringType = $this->a;

			while(1)
			{
				echo $this->a;
				$this->a = $this->getChar();

				switch($this->a)
				{
					case $stringType:
						break 2;

					case "\n":
						throw new JShrinkException('Unclosed string. ' . $this->index);
						break;

					case '\\':
						echo $this->a;
						$this->a = $this->getChar();
				}
			}
		}
	}

	protected function saveRegex()
	{
		echo $this->a . $this->b;

		while(($this->a = $this->getChar()) !== false)
		{
			if($this->a == '/')
				break;

			if($this->a == '\\')
			{
				echo $this->a;
				$this->a = $this->getChar();
			}

			if($this->a == "\n")
				throw new JShrinkException('Stray regex pattern. ' . $this->index);

			echo $this->a;
		}
		$this->b = $this->getReal();
	}

	static protected function isAlphaNumeric($char)
	{
		return preg_match('/^[\w\$]$/', $char) === 1 || $char == '/';
	}

}

// Adding a custom exception handler for your own projects just means changing this line
class JShrinkException extends Exception {}
?>