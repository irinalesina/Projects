﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Authorization
{
    public partial class AutorizForm : Form
    {
        

        public AutorizForm()
        {
            InitializeComponent();
        }


        private void AutorizForm_Load(object sender, EventArgs e)
        {


        }

        void tbPassword_GotFocus(object sender, EventArgs e)
        {
            if (tbPassword.Text == "Пароль")
            {
                tbPassword.ForeColor = Color.Black;
                tbPassword.Text = "";
                tbPassword.PasswordChar = '*';
            }
        }

        void tbPassword_LostFocus(object sender, EventArgs e)
        {
            if (tbPassword.Text.Length == 0)
            {
                tbPassword.ForeColor = Color.LightGray;
                tbPassword.Text = "Пароль";
                tbPassword.PasswordChar ='\0';
            }
        }

        void tbName_LostFocus(object sender, EventArgs e)
        {
            if (tbName.Text.Length==0)
           {
            tbName.ForeColor = Color.LightGray;
            tbName.Text = "Имя пользователя";
           }
        }

        void tbName_GotFocus(object sender, EventArgs e)
        {
            if (tbName.Text == "Имя пользователя")
            {
                tbName.ForeColor = Color.Black;
                tbName.Text = "";
            }
        }      

        private void linkPass_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.Hide();
            PasswordForm frmPass = new PasswordForm(this);
            frmPass.ShowDialog();
        }

        private void linkReg_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.Hide();
            RegistrForm frmReg = new RegistrForm(this);
            frmReg.Show();
        }

        private void btnIn_Click(object sender, EventArgs e)
        {
            if (DBConnector.FindLogin(tbName.Text) == false)
            {
                MessageBox.Show("User not find!");
            }
            else 
            {
                if(DBConnector.CheckUser(tbName.Text, tbPassword.Text))
                {
                    this.Hide();
                    MainForm mainForm = new MainForm(this);
                    mainForm.Show();
                }
                else
                {
                    MessageBox.Show("You enter invaled password!");
                }
            }
        }
    }

}