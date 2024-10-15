type cdfs_perm.txt >fs.txt
type cdfs_extra.txt >>fs.txt
for /a:d %%i in (data\missions\*) do type %%i\cdfs_mission.txt >> fs.txt
