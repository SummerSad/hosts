$list = @("http://winspyblocker.crazyws.fr/data/hosts/win10/spy.txt")
$data = "data"

[Net.ServicePointManager]::SecurityProtocol =  [System.Security.Authentication.SslProtocols] "tls, tls11, tls12"

# Ensure data exists
if(!(Test-Path -PathType Container $data)){
        New-Item -ItemType Directory -Force -Path $data
}

# Download hosts from various sources
for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile $data\raw -Uri $list[$i]
        Get-Content $data\raw | Out-File -Encoding ASCII $data\$i.txt
        Remove-Item $data\raw
}

# Remove old spy hosts
if(Test-Path -PathType leaf .\spy){
        Remove-Item .\spy
}

# Do the merge
$make = "mingw32-make.exe"
$merge = ".\merge_host.exe"
if (Get-Command $make -errorAction SilentlyContinue)
{
    & $make
}
if (Get-Command $merge -errorAction SilentlyContinue)
{
    & $merge .\spy $data\0.txt $data\sowc.txt $data\steve.txt
}
