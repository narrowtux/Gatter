#!/bin/bash
lupdate-4.5 Gatter.pro
/Developer/Applications/Qt/Linguist.app/Contents/MacOS/Linguist gatter*.ts
lrelease-4.5 Gatter.pro
cp gatter*.qm ../Gatter-build-desktop/Gatter.app/Contents/MacOS
