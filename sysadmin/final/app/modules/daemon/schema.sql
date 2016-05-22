CREATE TABLE users (
  username VARCHAR(255) NOT NULL,
  email VARCHAR(255) NOT NULL,
  address TEXT NOT NULL,
  token VARCHAR(255) DEFAULT NULL,
  PRIMARY KEY (username)
);
