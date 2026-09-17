Send To Raspeberry PI


mack
rsync -avz --delete --exclude test \
  /Users/merlcreps/university-toledo/eet3150/assignments/assignment-1/ \
  pi@192.168.1.100:/home/pi/assignment1/


PC
cd /d C:\Users\merl\university-toledo\eet3150\assignments\assignment-1
scp -r . pi@192.168.1.100:/home/pi/assignment1/

Putty for ssh into the Raspberry
