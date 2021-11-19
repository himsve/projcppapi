Option Explicit On

Imports System
Imports System.IO
Imports System.Runtime.InteropServices
Imports System.Text

Module Program

    Public Class Utilities

        ' TODO: Test
        Public Shared Function TT() As String
            Return ""
        End Function

        ' Declare Function proj_torad Lib "C:\Users\himsve\source\repos\proj_custom\Library\bin\proj_8_0.dll" Alias "proj_torad" (ByVal input As Double) As Double
        ' Declare Function proj_create Lib "C:\Users\himsve\source\repos\proj_custom\Library\bin\proj_8_0.dll" Alias "proj_create" (ByRef o As Byte(), ByRef crs As String) As Byte()
        ' Declare Sub proj_string_list_destroy Lib "C:\Users\himsve\source\repos\proj_custom\Library\bin\proj_8_0.dll" Alias "proj_string_list_destroy" (<MarshalAs(UnmanagedType.LPWStr)> ByVal lpFileName As String)

        Declare Function Foo Lib "ProjCppApi.dll" Alias "Foo" () As Double
        'Declare Function Foo Lib "C:\Users\himsve\source\repos\proj_custom\Library\lib\ProjCppApi.dll" Alias "Foo" () As Double

        Declare Function GetProjDbPath Lib "ProjCppApi.dll" Alias "GetProjDbPath" () As IntPtr
        ' Declare Function GetProjDbPath Lib "C:\Users\himsve\source\repos\proj_custom\Library\lib\ProjCppApi.dll" Alias "GetProjDbPath" () As IntPtr

        Declare Function SetProjDbPath Lib "ProjCppApi.dll" Alias "SetProjDbPath" (<MarshalAs(UnmanagedType.LPStr)> path As String) As Boolean
        ' Declare Function SetProjDbPath Lib "C:\Users\himsve\source\repos\proj_custom\Library\lib\ProjCppApi.dll" Alias "SetProjDbPath" (<MarshalAs(UnmanagedType.LPStr)> path As String) As Boolean

        Declare Sub ProjTransform Lib "ProjCppApi.dll" Alias "ProjTransform" _
        (<MarshalAs(UnmanagedType.LPStr)> sourceCrs As String,
        <MarshalAs(UnmanagedType.LPStr)> targetCrs As String,
        ByVal x As Double,
        ByVal y As Double,
        ByVal z As Double,
        ByVal e As Double,
        ByRef xOut As Double,
        ByRef yOut As Double,
        ByRef zOut As Double)
        'Declare Sub ProjTransform Lib "C:\Users\himsve\source\repos\proj_custom\Library\lib\ProjCppApi.dll" Alias "ProjTransform" _
        '(<MarshalAs(UnmanagedType.LPStr)> sourceCrs As String,
        '<MarshalAs(UnmanagedType.LPStr)> targetCrs As String,
        'ByVal x As Double,
        'ByVal y As Double,
        'ByVal z As Double,
        'ByVal e As Double,
        'ByRef xOut As Double,
        'ByRef yOut As Double,
        'ByRef zOut As Double)

    End Class

    Public Sub Main(args As String())

        ' Dim ut = Utilities.proj_torad(5)
        Dim foo As Double = Utilities.Foo()

        Dim s As IntPtr = Utilities.GetProjDbPath()
        Dim answerStr = Marshal.PtrToStringAnsi(s)
        Console.WriteLine(answerStr)

        Dim xIn As Double = 2289009.21694, yIn As Double = 673144.33386, zIn As Double = 5895302.48216, e As Double = 2020.0
        Dim xOut As Double = 0, yOut As Double = 0, zOut As Double = 0

        Utilities.ProjTransform("EPSG:7789", "EPSG:4936", xIn, yIn, zIn, e, xOut, yOut, zOut)
        Console.WriteLine($"Input:{vbCrLf}{xIn} {yIn} {zIn} {e}{vbCrLf}Output:{vbCrLf}{xOut} {yOut} {zOut}")

        Console.ReadLine()

    End Sub

End Module
