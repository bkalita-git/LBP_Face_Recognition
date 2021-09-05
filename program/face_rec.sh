python create_csv.py faces/friends
python create_test_images.py test/friends
python create_name.py faces/friends
g++ -I/usr/local/include/opencv2/ -I/usr/local/include/opencv -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_core project.C
./a.out



