<?php
	
	//Define o url do site
	define("BASE_PATH", "http://localhost");
	
	//Define a pasta onde estб o nosso projecto
	$path = "/my_mvc";
	
	//Retirar a path do inicнo e cria a var $url com o restante
	$url = $_SERVER['REQUEST_URI'];
	$url = 	str_replace($path,"",$url);
	
	//cria um array com o resto do url
	$array_tmp_uri = preg_split('[\\/]', $url, -1, PREG_SPLIT_NO_EMPTY);

	//Aqui vamos definir o que й representa o resto do URL
	$array_uri['controller'] 	= $array_tmp_uri[0]; //a class
	$array_uri['method']		= $array_tmp_uri[1]; //a funзгo
	$array_uri['var']			= $array_tmp_uri[2]; //a variavel
	
	//inclui a base do framework
	require_once("application/base.php");
	
	//carrega o controlador
	$application = new Application($array_uri);
	$application->loadController($array_uri['controller']);
	

?>