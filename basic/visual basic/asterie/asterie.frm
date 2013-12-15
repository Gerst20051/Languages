VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "AsterWin IE"
   ClientHeight    =   4620
   ClientLeft      =   1275
   ClientTop       =   1725
   ClientWidth     =   6135
   Icon            =   "asterie.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4620
   ScaleWidth      =   6135
   Begin VB.CommandButton cmdPasswords 
      Caption         =   "Show Internet Explorer Passwords"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   3960
      Width           =   2775
   End
   Begin VB.TextBox txtPasswords 
      Height          =   3735
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   120
      Width           =   5775
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
'AsterWin IE v1.03
'Copyright © 2002 - 2004 Nir Sofer
'
'Web site: http://www.nirsoft.net
'
'This utility reveals the passwords behind the asterisks in the Internet Explorer windows. (version 5.x and above only)
'It scans all opened Internet Explorer windows on your system, and reveals the passwords behind the asterisks in all password-boxes that appears in the web pages.
'
'License:
'This utility is released as freeware.
'You are allowed to freely distribute this utility via floppy disk, CD-ROM,
'Internet, or in any other way, as long as you don't charge anything for this.
'If you distribute this utility, you must include all files in the distribution
'package including the source code, without any modification !
'You are not allowed to combine this utility with a commercial product in any way !

Private strCurrTitle            As String

'This function checks if we can access the Document object without errors.
Private Function CanAccessDocumentObject(Obj As Object) As Boolean
    Dim oDocument       As Object
    On Error GoTo err1:
    Set oDocument = Obj.document
    Set oDocument = Nothing
    CanAccessDocumentObject = True
    Exit Function
err1:
    CanAccessDocumentObject = False
End Function

Private Function IsPasswordBox(objElement As Object) As Boolean
    On Error GoTo err1
    If LCase(objElement.getAttribute("Type")) = "password" Then
        IsPasswordBox = True
    Else
        IsPasswordBox = False
    End If
    Exit Function
err1:
    IsPasswordBox = False
End Function

Private Function SearchPasswordsInDoc(objDoc As Object) As Boolean
    Dim objElement      As Object
    Dim lngLen          As Long
    Dim lngIndex        As Long
    Dim blnFound        As Boolean
    
    'Get the number of elements in the document.
    lngLen = objDoc.All.length
    
    'Enumerates all elements in the document, in order to find the password elements.
    For Each objElement In objDoc.All
        DoEvents
        'Checks if the element is a password-box.
        If IsPasswordBox(objElement) Then
            'We found a password-box, so we reveal it together with window title.
            txtPasswords.Text = txtPasswords.Text & "Window Title: " & strCurrTitle & vbCrLf
            txtPasswords.Text = txtPasswords.Text & "Password: " & objElement.getAttribute("Value") & vbCrLf & vbCrLf
            blnFound = True
        End If
    Next
    
    lngLen = objDoc.frames.length
    
    'Enumerates all frames in the document
    For lngIndex = 0 To lngLen - 1
        'First, check if we can access the document object without receiving any error:
        If CanAccessDocumentObject(objDoc.frames.Item(lngIndex)) Then
            'If the document contains one or more frames, search for a password also in them:
            If SearchPasswordsInDoc(objDoc.frames.Item(lngIndex).document) Then blnFound = True
        End If
    Next
    
    SearchPasswordsInDoc = blnFound
End Function

Private Sub ScanPasswords()
    Dim objShellWins    As New SHDocVw.ShellWindows
    Dim objExplorer     As SHDocVw.InternetExplorer
    Dim objDocument     As HTMLDocument
    Dim blnFound        As Boolean
    Dim blnResult       As Boolean
    
    txtPasswords = "Scanning all windows, please wait..." & vbCrLf & vbCrLf
    Screen.MousePointer = vbHourglass
    'Enumerates All IE windows.
    For Each objExplorer In objShellWins
        If TypeOf objExplorer.document Is HTMLDocument Then
            Set objDocument = objExplorer.document
            'Saves the current title for using it if a password-box is found.
            strCurrTitle = objDocument.Title
            'Search for password-boxes in the document, including all frames in it.
            blnResult = SearchPasswordsInDoc(objDocument)
            If blnResult Then blnFound = True
        End If
    Next
    
    If Not blnFound Then
        txtPasswords.Text = txtPasswords.Text & "Password(s) not found." & vbCrLf
    Else
        txtPasswords.Text = txtPasswords.Text & "Done !" & vbCrLf
    End If
    Screen.MousePointer = vbDefault
End Sub

Private Sub cmdPasswords_Click()
    On Error GoTo err1
    ScanPasswords
    Exit Sub
err1:
    Screen.MousePointer = vbDefault
    MsgBox "Error " & CStr(Err.Number) & ": " & Err.Description, vbOKOnly Or vbExclamation, ""
End Sub
