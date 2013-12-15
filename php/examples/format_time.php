<?php
/* --> Dont paste this line <-- */
			$d = $time - $timestamp;
			if ($d < 60)
				return $d." second".(($d==1)?'':'s')." ago";
			else
			{
				$d = floor($d / 60);
				if($d < 60)
					return $d." minute".(($d==1)?'':'s')." ago";
				else
				{
					$d = floor($d / 60);
					if($d < 24)
						return $d." hour".(($d==1)?'':'s')." ago";
					else
					{
						$d = floor($d / 24);
						if($d < 7)
							return $d." day".(($d==1)?'':'s')." ago";
						else
						{
							$d = floor($d / 7);
							if($d < 4)
								return $d." week".(($d==1)?'':'s')." ago";
						}//Week
					}//Day
				}//Hour
			}//Minute
?>