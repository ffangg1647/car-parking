Public Class frm_login
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If TextBox1.Text = "Username" And TextBox2.Text = "Password" Then

            MsgBox("Successful Login!")
            Me.Hide()
            Main.Show()
        ElseIf TextBox1.Text = " " And TextBox2.Text = " " Then
            MsgBox("Please Enter Username & Password.")
        Else MsgBox("Username or Password was incorrect.", MsgBoxStyle.Critical, "Information")()
        End If
    End Sub
End Class