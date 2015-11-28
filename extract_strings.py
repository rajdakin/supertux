import sys
import re

pattern = re.compile(r"(translate|_)\((.*?)\)")

lisp_template_begin = """(msgids
""";
lisp_template_end = ")"

lisp_content = lisp_template_begin

if(len(sys.argv) < 3):
	print("Usage: " + sys.argv[0] + " <filename> <output file>")
else:
	filename = sys.argv[1]
	out_file = sys.argv[2]
	strings_found = False
	print("Extracting strings from " + filename + "...")
	with open(filename, 'r') as f:
		s = f.read()
	matches = pattern.findall(s)
	if len(matches) > 0:
		strings_found = True

	for match in matches:
		lisp_content += "  (msgid"
		lisp_content += " (_ " + match[1] + ")"
		lisp_content += ")\r\n"
	lisp_content += lisp_template_end
	print(lisp_content)

	if strings_found:
		output_file = open(out_file, "w")
		output_file.write(lisp_content)
		output_file.close()