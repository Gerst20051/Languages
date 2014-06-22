<?php

abstract class LongPoller {

	protected $sleepTime = 5;
	protected $timeoutTime = 30;

	function __construct() {}

	function setTimeout($timeout) {
		$this->timeoutTime = $timeout;
	}

	function setSleep($sleep) {
		$this->sleepTime = $sleepTime;
	}

	public function run() {
		$data = NULL;
		$timeout = 0;

		set_time_limit($this->timeoutTime + $this->sleepTime + 15);

		//Query database for data
		while($data == NULL && $timeout < $this->timeoutTime) {
			$data = $this->loadData();
			if($data == NULL){

				//No new orders, flush to notify php still alive
				flush();

				//Wait for new Messages
				sleep($this->sleepTime);
				$timeout += $this->sleepTime;
			}else{
				echo $data;
				flush();
			}
		}
	}

	protected abstract function loadData();
}

?>