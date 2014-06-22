<?php

abstract class LongPoller {

	protected $sleepTime = 5;
	protected $timeoutTime = 30;

	function __construct() {}

	function setTimeout($timeout) {
		$this->timeoutTime = $timeout;
	}

	function setSleep($sleep) {
		$this->sleepTime = $sleep;
	}

	public function run() {
		$data = null;
		$timeout = 0;

		set_time_limit($this->timeoutTime + $this->sleepTime + 15);

		while ($data == null && $timeout < $this->timeoutTime) {
			$data = $this->loadData();
			if ($data == null) {
				flush();
				sleep($this->sleepTime);
				$timeout += $this->sleepTime;
			} else {
				echo $data;
				flush();
			}
		}
	}

	protected abstract function loadData();
}

?>