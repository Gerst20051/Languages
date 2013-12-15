<?php
/**
 * WebSocket extension class of phpWebSockets
 *
 * @author Moritz Wutz <moritzwutz@gmail.com>
 * @version 0.1
 * @package phpWebSockets
 */

class socketWebSocket extends socket
{
	private $clients = array();
	private $handshakes = array();

	public function __construct()
	{
		parent::__construct();

		$this->run();
	}

	/**
	 * Runs the while loop, wait for connections and handle them
	 */
	private function run()
	{
		while(true)
		{
			# because socket_select gets the sockets it should watch from $changed_sockets
			# and writes the changed sockets to that array we have to copy the allsocket array
			# to keep our connected sockets list
			$changed_sockets = $this->allsockets;

			# blocks execution until data is received from any socket
			$num_sockets = socket_select($changed_sockets,$write=NULL,$exceptions=NULL,NULL);

			# foreach changed socket...
			foreach( $changed_sockets as $socket )
			{
				# master socket changed means there is a new socket request
				if( $socket==$this->master )
				{
					# if accepting new socket fails
					if( ($client=socket_accept($this->master)) < 0 )
					{
						console('socket_accept() failed: reason: ' . socket_strerror(socket_last_error($client)));
						continue;
					}
					# if it is successful push the client to the allsockets array
					else
					{
						$this->allsockets[] = $client;

						# using array key from allsockets array, is that ok?
						# i want to avoid the often array_search calls
						$socket_index = array_search($client,$this->allsockets);
						$this->clients[$socket_index] = new stdClass;
						$this->clients[$socket_index]->socket_id = $client;

						$this->console($client . ' CONNECTED!');
					}
				}
				# client socket has sent data
				else
				{
					$socket_index = array_search($socket,$this->allsockets);

					# the client status changed, but theres no data ---> disconnect
					$bytes = @socket_recv($socket,$buffer,2048,0);
					if( $bytes === 0 )
					{
						$this->disconnected($socket);
					}
					# there is data to be read
					else
					{
						# this is a new connection, no handshake yet
						if( !isset($this->handshakes[$socket_index]) )
						{
							$this->do_handshake($buffer,$socket,$socket_index);
						}
						# handshake already done, read data
						else
						{
							$action = substr($buffer,1,$bytes-2); // remove chr(0) and chr(255)
							$this->console("<{$action}");
							

							if( method_exists('socketWebSocketTrigger',$action) )
							{
								$this->send($socket,socketWebSocketTrigger::$action());
							}
							
							else
							{
								for ($i = 0; $i <= 0; $i++) {
									$this->send($socket,"{$action}");
								}
							}

						}
					}
				}
			}
		}
	}

	/**
	 * Manage the handshake procedure
	 *
	 * @param string $buffer The received stream to init the handshake
	 * @param socket $socket The socket from which the data came
	 * @param int $socket_index The socket index in the allsockets array
	 */
	private function do_handshake($buffer,$socket,$socket_index)
	{
		$this->console('Requesting handshake...');

		list($resource,$host,$origin) = $this->getheaders($buffer);

		$this->console('Handshaking...');

		$upgrade  = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n" .
				"Upgrade: WebSocket\r\n" .
				"Connection: Upgrade\r\n" .
				"WebSocket-Origin: {$origin}\r\n" .
				"WebSocket-Location: ws://{$host}{$resource}\r\n\r\n" . chr(0);

		$this->handshakes[$socket_index] = true;

		socket_write($socket,$upgrade,strlen($upgrade));

		$this->console('Done handshaking...');
	}

	/**
	 * Extends the socket class send method to send WebSocket messages
	 *
	 * @param socket $client The socket to which we send data
	 * @param string $msg  The message we send
	 */
	protected function send($client,$msg)
	{
		$this->console(">{$msg}");

		parent::send($client,chr(0).$msg.chr(255));
	}

	/**
	 * Disconnects a socket an delete all related data
	 *
	 * @param socket $socket The socket to disconnect
	 */
	private function disconnected($socket)
	{
		$index = array_search($socket, $this->allsockets);
		if( $index >= 0 )
		{
			unset($this->allsockets[$index]);
			unset($this->clients[$index]);
			unset($this->handshakes[$index]);
		}

		socket_close($socket);
		$this->console($socket." disconnected!");
	}

	/**
	 * Parse the handshake header from the client
	 *
	 * @param string $req
	 * @return array resource,host,origin
	 */
	private function getheaders($req)
	{
		$req  = substr($req,4); /* RegEx kill babies */
		$res  = substr($req,0,strpos($req," HTTP"));
		$req  = substr($req,strpos($req,"Host:")+6);
		$host = substr($req,0,strpos($req,"\r\n"));
		$req  = substr($req,strpos($req,"Origin:")+8);
		$ori  = substr($req,0,strpos($req,"\r\n"));

		return array($res,$host,$ori);
	}

	/**
	 * Extends the parent console method.
	 * For now we just set another type.
	 *
	 * @param string $msg
	 * @param string $type
	 */
	protected function console($msg,$type='WebSocket')
	{
		parent::console($msg,$type);
	}
}

?>