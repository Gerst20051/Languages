<?php
include_once("model/Book.php");

class Model {
	public function getBookList() {
		// here goes some hardcoded values to simulate the database
		return array(
			"Jungle Book" => new Book("Jungle Book","R. Kipling","A classic book."),
			"Moonwalker" => new Book("Moonwalker","J. Walker",""),
			"PHP for Dummies" => new Book("PHP for Dummies","Some Smart Guy","")
		);
	}
	
	public function getBook($title) {
		// we use the precious function to get all the books and then we return the requested one
		// in a real life scenario this will be done through a db select command
		$allBooks = $this->getBookList();
		return $allBooks[$title];
	}
}
?>