# 285C-stable

Backup barebones code for 285C

# Building

## 1. Compile the code

To build the code, run:

```
prosv5 make
```

## 2. Upload the binary

To upload the code to a brain or a controller, run:

```
prosv5 upload
```

## 3. Run the code

1. On the controller, using arrow keys to scroll, select the `Programs` tab.
2. Scroll until you select the program you just uploaded.
3. Press the `A` button to run the program.

# Contributing

Since this is hosted on GitHub, please follow the best practices for code collaboration outlined in [this document](https://cdn.discordapp.com/attachments/1062157317208035333/1062196035021181000/codeCollab.pdf).

Here's a quick summary:

## Make sure to make a branch for your work before you start.

1. Open a Command Prompt or PowerShell window to the repository directory.
2. Create a new branch.
   ```
   git checkout -b <branch-name>
   ```
3. Make and save any changes in the directory now. They will be tracked.
4. Once a file is saved or changed, add and commit your changes.
   ```
   git add .
   git commit -m "<message-text>"
   ```
5. If someone else makes changes to any branch, update and merge your local copy with the remote changes.
   ```
   git pull
   ```
6. Repeat steps 3 and 5 until your code is complete.

## When you are done, upload your code and make a pull request back to the main branch.

1. Open a Command Prompt or PowerShell window to the repository directory.
2. "Register" your new branch with GitHub and push any changes to it.
   ```
   git push -u origin <branch-name>:<branch-name>
   ```
   > You must run the above command first before you can run the truncated command for subsequent pushes. Here's what it looks like:
   >
   > ```
   > git push
   > ```
3. On GitHub, open your branch.
4. Create a pull request by clicking the link in `x commits ahead of master`.
5. Review that the information is correct and that the arrow is pointing from your branch to the master branch.
   > This would be correct:
   >
   > ```
   > base: master <- compare: <branch-name>
   > ```
6. Create the pull request, but **do not merge to master**.
