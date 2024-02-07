# You don't need to run this for the game to work, although it runs by default if you have the interpreter installed

# A python script that just cleans the log file for easier readability
# I chose to use python because its so much easier to deal with strings here AND
# Im assuming you have the interpreter installed

# Read the contents of the log file first
log_file = open("build/logs/.log.txt", "r")
log_file_contents = log_file.readlines()
log_file_output = "";

# How many logs there are
log_count = 0

# Continue only if it hasn't been formatted
if(log_file_contents[0] != "<formatted>"):

  # Parse and format
  for i in range(len(log_file_contents)):
    if("[-" in log_file_contents[i]):

      # Count, warning, description
      log_warning = log_file_contents[i].split("[-")[1][0:-1]
      log_location = log_file_contents[i].split("[-")[0].split(" ")[0][0:-1]
      log_description = log_file_contents[i].split("[-")[0].split(":")[4]

      # Displays the warning but formatted in a much more concise manner
      log_file_output += "({c:2d}) [-{w:32s} {l:40s} {d:s}\n".format(c = log_count, w = log_warning, l = log_location, d = log_description)
      log_count += 1

    # Spit out the errors too
    elif("error" in log_file_contents[i]):
      log_location = log_file_contents[i].split(" ")[0][0:-1]
      log_description = log_file_contents[i].split(":")[4][0:-1]

      log_file_output += "({c:2d}) [-{e:32s} {l:40s} {d:s}\n".format(c = log_count, e = "error]", l = log_location, d = log_description)
      log_count += 1

  # Write to log file
  log_file = open("build/logs/.log.txt", "w")
  log_file.write("<formatted>\n");
  log_file.write(log_file_output);