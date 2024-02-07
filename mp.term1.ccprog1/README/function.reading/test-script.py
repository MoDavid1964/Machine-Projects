import os

root = '.\\src'

testScript = open('testScriptTable.txt', 'w')

counter = 1

for subdir, dirs, files in os.walk(root):
  for file in files:
    h_file = open(os.path.join(subdir, file), 'r')
    h_file_contents = h_file.readlines()
    
    # The start of a new file
    testScript.write('<table>\n');
    testScript.write('\t<tr>\n');
    testScript.write('\t\t<th> # </th>\n');
    testScript.write('\t\t<th colspan="2"> Functions in <code>{}</code> </th>\n'.format(file));
    testScript.write('\t\t<th colspan="4"> Description </th>\n');
    testScript.write('\t</tr>\n');

    index = 0
    
    for i in h_file_contents:
      if len(i.lstrip()) == len(i):
        if('#' not in i and '/' not in i and '}' not in i and ';' not in i):
          if('(' in i):
            desc = ''
            line = index

            # Grab the JS Doc
            while(line > 0):
              if('/*' in h_file_contents[line]):
                break
              line -= 1

            if(line == 0):
              continue;
            else:
              line += 1
              while('*/' not in h_file_contents[line] and '@' not in h_file_contents[line]):
                desc += h_file_contents[line].lstrip().lstrip('*')
                if(len(h_file_contents[line].lstrip().lstrip('*').strip()) == 0):
                  desc += '<br>'
                line += 1

            # Create table headets
            testScript.write('\t<tr>\n');
            testScript.write('\t\t<td>{}</td>\n'.format(counter));
            if('{' in i):
              if(i.count(',') < 1):
                testScript.write('\t\t<td colspan="3"><pre style="background: 0; white-space: pre-wrap;">{}</pre></td>\n'.format(i.split('{')[0]));
              else:
                testScript.write('\t\t<td colspan="3"><pre style="background: 0; white-space: pre-wrap;">{}</pre></td>\n'.format(i.split('{')[0].replace('(', '(\n\t').replace(', ', ',\n\t').replace(')', '\n)')));
            else:
              testScript.write('\t\t<td colspan="3"><pre style="background: 0; white-space: pre-wrap;">FIX THIS: {}</pre></td>\n'.format(i.replace('\n', '')));
            testScript.write('\t\t<td colspan="3">{}</td>\n'.format(desc));
            testScript.write('\t</tr>\n');

            counter += 1

      index += 1
    
    testScript.write('</table>\n\n');

testScript.close();