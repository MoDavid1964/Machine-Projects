import os

root = '.\\src'

testScript = open('testScriptTable.txt', 'w')

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

    counter = 1
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
              desc = 'no JS DOC description found.'
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
                testScript.write('\t\t<td colspan="2"><pre style="background: 0;">{}</pre></td>\n'.format(i.split('{')[0]));
              else:
                testScript.write('\t\t<td colspan="2"><pre style="background: 0;">{}</pre></td>\n'.format(i.split('{')[0].replace('(', '(\n\t').replace(', ', ',\n\t').replace(')', '\n)')));
            else:
              testScript.write('\t\t<td colspan="2"><pre style="background: 0;">FIX THIS: {}</pre></td>\n'.format(i.replace('\n', '')));
            testScript.write('\t\t<td colspan="4">{}</td>\n'.format(desc));
            testScript.write('\t</tr>\n');
            
            testScript.write('\t<tr>\n');
            testScript.write('\t\t<td colspan="3"></td>\n');
            testScript.write('\t\t<td><i><b>Sample Input</i></b></td>\n');
            testScript.write('\t\t<td><i><b>Expected Result</i></b></td>\n');
            testScript.write('\t\t<td><i><b>Actual Result</i></b></td>\n');
            testScript.write('\t\t<td><i><b>Pass / Fail</i></b></td>\n');
            testScript.write('\t</tr>\n');

            for j in range(3):
              testScript.write('\t<tr>\n');
              testScript.write('\t\t<td colspan="3"></td>\n');
              testScript.write('\t\t<td><code></code></td>\n');
              testScript.write('\t\t<td><code></code></td>\n');
              testScript.write('\t\t<td><code></code></td>\n');
              testScript.write('\t\t<td><code></code></td>\n');
              testScript.write('\t</tr>\n');

            counter += 1

      index += 1
    
    testScript.write('</table>\n\n');

testScript.close();