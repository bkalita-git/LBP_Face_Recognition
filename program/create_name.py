import sys
import os.path
if __name__ == "__main__":
    
    if len(sys.argv) != 2:
        print "usage: create_csv <base_path>"
        sys.exit(1)
    
    BASE_PATH=sys.argv[1]
    f= open("name.txt","w+")
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for subdirname in dirnames:
		f.write("%s\n" % (subdirname))
f.close()
