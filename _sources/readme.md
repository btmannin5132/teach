

This site is made primarily in Jupyter Books, more documentation can be found here: https://jupyterbook.org/en/stable/intro.html

Site can be found here: https://btmannin5132.github.io/teach/

Steps for publishing:

Install jupyter books if needed: (I am using an older version and didn't feel like updating)

$ `pip install -U 'jupyter-book<2'`

$ `pip install 'ghp-import<2'`

Create book if needed:
$ `jupyter-book create mybook`

After site modification in MAIN

Build: 

$ `jupyter-book clean .`

$ `jupyter-book build .`

Preview:

$`python3 -m http.server --directory _build/html`

If on Codespaces, a pop up will open asking to view in another tab

Push:

`git add .`

`git commit -m` assuming you are in the root foler

`git push`

Publish:

$`ghp-import -n -p -f _build/html`

There is also a makefile in the main directory that will complete this as needed.
