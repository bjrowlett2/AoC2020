param([Boolean] $RebuildAll = $true)

if ($RebuildAll) {
    .\BuildAll.cmd > $null 2> $null
}

function Check-Solution([String] $Day, [Int64] $Solution1, [Int64] $Solution2) {
    Push-Location "Day_${Day}\"

    $Output = .\.bin\Solution.exe | Out-String
    if ($Output -Match "Part 1: ${Solution1}") {
        Write-Host "Day ${Day}, Part 1 :: OK" -ForegroundColor Green
    } else {
        Write-Host "Day ${Day}, Part 1 :: FAILED" -ForegroundColor Red
    }

    if ($Output -Match "Part 2: ${Solution2}") {
        Write-Host "Day ${Day}, Part 2 :: OK" -ForegroundColor Green
    } else {
        Write-Host "Day ${Day}, Part 2 :: FAILED" -ForegroundColor Red
    }

    Pop-Location
}

Check-Solution -Day "01" -Solution1 1010884 -Solution2 253928438
Check-Solution -Day "02" -Solution1 660 -Solution2 530
Check-Solution -Day "03" -Solution1 280 -Solution2 4355551200
Check-Solution -Day "04" -Solution1 264 -Solution2 224
Check-Solution -Day "05" -Solution1 906 -Solution2 519
Check-Solution -Day "06" -Solution1 6249 -Solution2 3103
Check-Solution -Day "07" -Solution1 316 -Solution2 11310
Check-Solution -Day "08" -Solution1 1584 -Solution2 920
Check-Solution -Day "09" -Solution1 393911906 -Solution2 59341885
Check-Solution -Day "10" -Solution1 2201 -Solution2 169255295254528
Check-Solution -Day "11" -Solution1 2273 -Solution2 2064
Check-Solution -Day "12" -Solution1 441 -Solution2 40014
Check-Solution -Day "13" -Solution1 3966 -Solution2 800177252346225
Check-Solution -Day "14" -Solution1 13727901897109 -Solution2 5579916171823
Check-Solution -Day "15" -Solution1 1194 -Solution2 48710
Check-Solution -Day "16" -Solution1 24110 -Solution2 6766503490793
Check-Solution -Day "17" -Solution1 306 -Solution2 2572
Check-Solution -Day "18" -Solution1 24650385570008 -Solution2 158183007916215
