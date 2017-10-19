CREATE TABLE ins
(
  device_id INTEGER PRIMARY KEY,
  loc_x REAL,
  loc_y REAL,
  loc_z REAL,
  ssid1 TEXT,
  rss1 REAL,
  ssid2 TEXT,
  rss2 REAL,
  ssid3 TEXT,
  rss3 REAL,
  ssid4 TEXT,
  rss4 REAL,
  ssid5 TEXT,
  rss5 REAL,
  timestamp datatime default current_timestamp
);
CREATE TABLE dev_1000
(
  id INTEGER PRIMARY KEY,
  ssid1 TEXT,
  rss1 REAL,
  ssid2 TEXT,
  rss2 REAL,
  ssid3 TEXT,
  rss3 REAL,
  ssid4 TEXT,
  rss4 REAL,
  ssid5 TEXT,
  rss5 REAL,
  timestamp datatime default current_timestamp
);
CREATE TABLE location
(
  device_id INTEGER PRIMARY KEY,
  employee_id TEXT,
  loc_x REAL,
  loc_y REAL,
  loc_z REAL,
  timestamp datatime default current_timestamp
);
CREATE TABLE dev_1001
(
  id INTEGER PRIMARY KEY,
  ssid1 TEXT,
  rss1 REAL,
  ssid2 TEXT,
  rss2 REAL,
  ssid3 TEXT,
  rss3 REAL,
  ssid4 TEXT,
  rss4 REAL,
  ssid5 TEXT,
  rss5 REAL,
  timestamp datatime default current_timestamp
);
CREATE TABLE dev_1300
(
  id INTEGER PRIMARY KEY,
  ssid1 TEXT,
  rss1 REAL,
  ssid2 TEXT,
  rss2 REAL,
  ssid3 TEXT,
  rss3 REAL,
  ssid4 TEXT,
  rss4 REAL,
  ssid5 TEXT,
  rss5 REAL,
  timestamp datatime default current_timestamp
);