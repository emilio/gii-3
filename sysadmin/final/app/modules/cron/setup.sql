DROP DATABASE app;
DROP USER app;

/* TODO Don't hardcode this */
CREATE USER app
  WITH PASSWORD '12345678';

CREATE DATABASE app OWNER app;
