create table users (
    userID int(11) unsigned not null auto_increment,    
    email char(45) not null,
    password char(15) not null,
    fname char(30) not null,
    lname char(30) not null,
    posted datetime not null,
    status enum('active','disabled') default 'active',
    PRIMARY KEY (userID),
    UNIQUE (email),
    KEY (posted),
    KEY (status)
) ENGINE=InnoDB;

insert into users (email,password,fname,lname,posted,status) values ('anon@example.com','','Anonymous','Coward',NOW(),'disabled');

