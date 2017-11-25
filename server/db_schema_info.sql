-- Stores location records --
CREATE TABLE location
(
  device_id INTEGER PRIMARY KEY,
  employee_id TEXT,
  pos_x REAL,
  pos_y REAL,
  pos_z REAL,
  timestamp datatime default current_timestamp
);

-- Stores wifi access points information --
CREATE TABLE access_points
(
  mac_addr TEXT PRIMARY KEY,
  pos_x REAL,
  pos_y REAL,
  pos_z REAL
);

-- Stores mac_addr/rssi info from device id - dev_10 --
CREATE TABLE sample_dev_10
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);

CREATE TABLE sample_dev_11
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);


CREATE TABLE sample_dev_1034
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);


CREATE TABLE sample_dev_103
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);
