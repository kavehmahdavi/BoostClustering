set datafile separator ","

set title "Cluster Analysis Results of trace 'gromacs_64p.chop1.prv'\nDBSCAN (Eps=0.0165, MinPoints=8)"
set xlabel "IPC"
set ylabel "Instructions Completed"
set xrange [:]
set yrange [:]

set key right outside
plot 'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 5 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#b30000" title "Noise",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 6 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#00ff00" title "Cluster 1",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 7 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#ffff00" title "Cluster 2",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 8 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#eb0000" title "Cluster 3",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 9 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#00a200" title "Cluster 4",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 10 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#ff00ff" title "Cluster 5",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 11 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#6464b1" title "Cluster 6",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 12 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#acae29" title "Cluster 7",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 13 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#ff901a" title "Cluster 8",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 14 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#02ffb1" title "Cluster 9",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 15 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#c0e000" title "Cluster 10",\
'gromacs_64p.chop1.clustered.DATA.csv' using 9:($20 == 16 ? $8 : 1/0) w points ps 1.5 lt rgbcolor "#424242" title "Cluster 11"
pause -1 "Press return to continue..."
