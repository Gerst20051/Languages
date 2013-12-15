<?php
	class Blog extends Application
	{
		function __construct()
		{
			$this->loadModel('model_blog');
		}
		
		function index()
		{
			$artigos = $this->model_blog->select();
			$data['artigos'] = $artigos;
			$this->loadView('view_blog',$data);
		}
		
		function add($titulo="")
		{
			$data['titulo'] = $titulo;
			$this->loadView('view_blog_add',$data);
		}
		
	}
?>