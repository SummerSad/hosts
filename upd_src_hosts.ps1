$list = @("http://someonewhocares.org/hosts/zero/hosts",
        "http://winhelp2002.mvps.org/hosts.txt",
        "https://raw.githubusercontent.com/hoshsadiq/adblock-nocoin-list/master/hosts.txt",
        "https://raw.githubusercontent.com/ZeroDot1/CoinBlockerLists/master/hosts",
        "https://pgl.yoyo.org/adservers/serverlist.php?hostformat=hosts&mimetype=plaintext&useip=0.0.0.0",
        "https://raw.githubusercontent.com/mitchellkrogza/Badd-Boyz-Hosts/master/hosts",
        "https://raw.githubusercontent.com/azet12/KADhosts/master/KADhosts.txt",
        "https://raw.githubusercontent.com/StevenBlack/hosts/master/data/StevenBlack/hosts",
        "https://raw.githubusercontent.com/thisisu/hosts/master/hosts",
        "https://raw.githubusercontent.com/CHEF-KOCH/Spotify-Ad-free/master/Spotifynulled.txt",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/SpotifyAds/hosts",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/UncheckyAds/hosts",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/add.2o7Net/hosts",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/add.Dead/hosts",
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/add.Risk/hosts"
        "https://raw.githubusercontent.com/FadeMind/hosts.extras/master/add.Spam/hosts",
        "https://raw.githubusercontent.com/crazy-max/WindowsSpyBlocker/master/data/hosts/win7/spy.txt",
        "https://raw.githubusercontent.com/crazy-max/WindowsSpyBlocker/master/data/hosts/win81/spy.txt",
        "https://raw.githubusercontent.com/crazy-max/WindowsSpyBlocker/master/data/hosts/win10/spy.txt",
        "https://raw.githubusercontent.com/tyzbit/hosts/master/data/tyzbit/hosts",
        "https://raw.githubusercontent.com/Yhonay/antipopads/master/hosts")

for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile .\hosts\raw -Uri $list[$i]
        Get-Content .\hosts\raw | Out-File -Encoding ASCII .\hosts\$i.txt
        Remove-Item .\hosts\raw
}

.\merge_host.exe raw .\hosts\0.txt .\hosts\1.txt .\hosts\2.txt .\hosts\3.txt .\hosts\4.txt
.\merge_host.exe raw .\hosts\5.txt .\hosts\6.txt .\hosts\7.txt .\hosts\8.txt .\hosts\9.txt
.\merge_host.exe raw .\hosts\10.txt .\hosts\11.txt .\hosts\12.txt .\hosts\13.txt .\hosts\14.txt
.\merge_host.exe raw .\hosts\15.txt .\hosts\16.txt .\hosts\17.txt .\hosts\18.txt .\hosts\19.txt
.\merge_host.exe raw .\hosts\20.txt