# Security Camera using OpenCV 4.2.0

Uses difference between pictures taken with a configurable delay in between to determine if there was a motion.
Whenever a motion is triggered the timestamp and a WARNING is logged, an image gets captured and saved with a timestamp in its filename.

### Used VS2019 and OpenCV 4.2.0

###### Code is Windows dependent(university assignment). Replace getDateTimeNow with a non-Windows implementation to run on a different OS.
