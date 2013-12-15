<?php
function _networkProbe ($noTrials=100, $startPortNumber=55000) {
	$endPortNumber = $startPortNumber + $noTrials;

	// temporarily set timeout to 2 seconds. we'll modify this with the
	// data that we get from this method

	$this-> timeout = 2;

	// setup a for loop to scan the ports

	for ($portNumber = $startPortNumber; $portNumber < $endPortNumber; $portNumber++) {
		$startTime = $this-> _getmicrotime();
		$result = $this-> _scanPort($portNumber);
		$endTime = $this-> _getmicrotime();
		$timeDiff = $endTime - $startTime;

		if (!$result) {
			$responsesArray[] = $timeDiff;
			$totalTime += $timeDiff;
		}
	}

	$noResponses = count($responsesArray);

	// if more than 40% of the datagrams timed out, abort the scan

	if ($noResponses < (.6 * $noTrial)) {
		echo "The connection is losing too many packets. Scan aborted. <br/>";
		exit;
	}

	$averageResponseTime = $this-> _calcAvgResponseTime ($noResponses, $totalTime);
	$standardDeviation = $this-> _calcStdrDeviation ($responsesArray);

	// calculate the timeout value

	$timeoutValue = ceil($averageResponseTime + 4 * $standardDeviation);

	// calculate number of cleanup iterations we'll need
	// percentFalsePositive is the % of datagrams that we sent in
	// the trial that timed out

	$percentFalsePositives = ($noTrials - $noResponses)/$noTrials;

	// percentResponses is the % of datagrams that we sent in the trial
	// that returned (eg -- didn't timeout)

	$percentResponses = $noResponses/$noTrials;

	// calculate the total number of ports to be scanned in the
	// real scan

	$portRange = $this-> maxPort - $this-> minPort + 1;

	// estFalsePositives is the estimated number of false positives we
	// anticipate getting from the real scan

	$estFalsePositives = $portRange * $percentFalsePositives;

	$this-> cleanupIterations = $this-> _calcNoIterations ($estFalsePositives, $percentResponses, $portRange);

	if ($this-> debug == 1) {
		echo "<br/>";
		echo "total time $totalTime<br/>";
		echo "timeout value: " . $this-> timeout . "<br/>";
		echo "cleanup iterations: " . $this-> cleanupIterations . "<br/>";
		echo "<br/>";
		flush();
	}
}
?>