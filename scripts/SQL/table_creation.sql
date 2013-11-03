DROP DATABASE IF EXISTS jmjrdf;
CREATE DATABASE jmjrdf;
USE jmjrdf;

CREATE TABLE IF NOT EXISTS c1(
	id binary(16),
	data mediumblob
) ENGINE=InnoDB default charset=utf8;

CREATE TABLE IF NOT EXISTS c2(
	id binary(16),
	data mediumblob
) ENGINE=InnoDB default charset=utf8;

CREATE TABLE IF NOT EXISTS c3(
	id binary(16),
	data mediumblob
) ENGINE=InnoDB default charset=utf8;

CREATE TABLE IF NOT EXISTS facts(
	id1 binary(16),	
	id2 binary(16),
	id3 binary(16)
) ENGINE=InnoDB default charset=utf8;

