import sys
import os.path
if __name__ == "__main__":
    
    if len(sys.argv) != 2:
        print "usage: create_csv <base_path>"
        sys.exit(1)
    
    BASE_PATH=sys.argv[1]
    SEPARATOR=";"

    label = 0
    f= open("test_images.txt","w+")
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for subdirname in dirnames:
            subject_path = os.path.join(dirname, subdirname)
            for filename in os.listdir(subject_path):
                abs_path = "%s/%s" % (subject_path, filename)
		f.write("%s\n" % (abs_path))
            label = label + 1
f.close()
