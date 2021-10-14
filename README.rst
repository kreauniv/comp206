Welcome to Comp206 docs
=======================

This directory is a render of the html documentation of year2021
course material for comp206. Follow the steps below to produce and
update the contents of this directory... assuming your most recent
changes are already committed and pushed to ``main`` branch.

.. code-block:: bash

    git checkout main
    pushd year2021
    make html
    pushd build/html
    tar zcvf /tmp/docs.tar.gz *
    popd
    popd
    git checkout gh-pages
    tar zxvf /tmp/docs.tar.gz
    git add .
    git commit -m "Updated course material"
    git push
    git checkout main
