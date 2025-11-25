
📝 Team Git Cheat Sheet

Follow this guide to ensure our code stays organized and we avoid conflicts.

🟢 Phase 1: Before You Start Working

Goal: Make sure you are working on a fresh slate with the latest code.
Decide what you are working on:
Pick a specific feature or bug fix (e.g., "Fix login button" or "Add user profile").
Update your local project:
Make sure you are on the main folder before updating.
Bash
git checkout main
git pull origin main


Create your workspace (Branch):
Create a specific branch for your task. Do not work directly on main.
Naming convention idea: name/feature (e.g., alex/login-fix).
Bash
git checkout -b <your-branch-name>


🟡 Phase 2: After You Are Done Coding

Goal: Save your work and send it to the cloud.
Check your changes:
See which files you modified.
Bash
git status


Stage your files:
Prepare the files to be saved. (The . adds everything, or you can type specific filenames).
Bash
git add .


Save your snapshot (Commit):
Write a short, clear message about what you accomplished.
Bash
git commit -m "Added the new login button styles"


Safety Sync:
Get the latest updates from the team one last time to ensure no conflicts arose while you were working.
Bash
git pull origin main


Note: If a screen pops up asking for a message, you can usually type :q and hit Enter to exit it.
Upload to GitHub (Push):
Send your branch to the remote server.
Bash
git push -u origin <your-branch-name>


🔵 Phase 3: The GitHub Website

Goal: Merge your code into the main project.
Open GitHub:
Go to our repository on your web browser.
Find the Alert:
*
Look for a yellow/green banner at the top that says "Your branch had recent pushes".
Click the green button that says Compare & pull request.
Create Pull Request (PR):
Review the title and description.
Click Create pull request.
Merge:
Wait for the checks to pass.
If there are No Conflicts, click Merge Pull Request -> Confirm Merge.

🆘 Troubleshooting / Quick Tips

"I'm stuck in a text editor (Vim)!"
Type :q! and hit Enter to force quit without saving.
"I have merge conflicts!"
Don't panic. Open the files in your code editor (VS Code/IntelliJ), look for the <<<<<<< headers, decide which code to keep, save the file, and run git add . and git commit again.


