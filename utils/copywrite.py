import os, sys, glob

old_copywrite_text = """/*
 * Copywrite
 */
"""

copywrite_text = """/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
"""

def process_file(file, do_execute):
	print 'analysing ' + file + '...',
	if not do_execute:
		print '(test only)',
	f = open(file, 'r')
	content = f.read()
	f.close()

	if content.startswith(old_copywrite_text):
		print 'replacing' 
		if do_execute:
			f = open(file, 'w')
			f.write(copywrite_text + content[len(old_copywrite_text):])
			f.close()
	elif not content.startswith(copywrite_text):
		print 'writing'
		if do_execute:
			f = open(file, 'w')
			f.write(copywrite_text + content)
			f.close()
	else:
		print 'skipping'

def list_files(path):
	filenames = []
	for root, dirs, files in os.walk(path):
		for file in files:
			if file.endswith('.h') or file.endswith('.cpp'):
				fullpath = os.path.join(root, file)
				filenames.append(fullpath)
	return filenames

def copywrite(files, do_execute):
	for file in files:
		process_file(file, do_execute)

def check(files):
	for file in files:
		print 'checking ' + file + '...',
		f = open(file, 'r')
		content = f.read()
		f.close()
		if content.startswith(old_copywrite_text):
			print 'has old copywrite'
		elif not content.startswith(copywrite_text):
			print 'doesn\'t have copywrite'
		else:
			print 'ok'

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print 'usage: ' + sys.argv[0] + ' (test|execute) [path]'
		sys.exit(0)

	path = sys.argv[2]
	do_execute = sys.argv[1] == 'execute'
	command = sys.argv[1]
	print 'running copywrite ' + command + ' on ' + path

	files = list_files(path)

	if command == "check":
		check(files)
	else:
		copywrite(files, do_execute)
