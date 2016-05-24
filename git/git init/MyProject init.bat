@echo off
cd C:\Users\tanak\Documents\Unreal Projects\MyProject
git init
echo MyProject.VC.db >> .gitignore
echo MyProject.VC.VC.opendb >> .gitignore
echo .vs >> .gitignore
echo Saved >> .gitignore
git add --all
git commit -a -m "‰Šú‰»"
git remote add origin //DESKTOP-72AJ5GU/1.git
git push origin master
