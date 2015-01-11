# Generating screencast

    $ ffcast -f -w
    $ ffmpeg -i file.mkv -ss 00:10 -r 4 anim.gif
    $ gifsicle -d 10 -O3 anim.gif opt.gif