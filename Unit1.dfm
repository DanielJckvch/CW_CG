object Form1: TForm1
  Left = 195
  Top = 143
  Width = 1135
  Height = 589
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = Form1Close
  OnCreate = Form1Create
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 550
    Height = 550
    OnMouseMove = Image1MouseMove
  end
  object Button1: TButton
    Left = 600
    Top = 24
    Width = 75
    Height = 25
    Caption = #1055#1086#1074' '#1087#1088#1086#1090' '#1095#1072#1089
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 784
    Top = 24
    Width = 75
    Height = 25
    Caption = #1055#1086#1074#1086#1088' '#1087#1086' '#1095#1072#1089
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 600
    Top = 88
    Width = 75
    Height = 25
    Caption = #1052#1072#1089#1096#1090#1072#1073' -'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 784
    Top = 88
    Width = 75
    Height = 25
    Caption = #1052#1072#1089#1096#1090#1072#1073' +'
    TabOrder = 3
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 600
    Top = 152
    Width = 75
    Height = 25
    Caption = '- '#1087#1086' '#1086#1089#1080
    TabOrder = 4
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 784
    Top = 152
    Width = 75
    Height = 25
    Caption = '+ '#1087#1086' '#1086#1089#1080
    TabOrder = 5
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 600
    Top = 248
    Width = 73
    Height = 25
    Caption = #1054#1089#1100
    TabOrder = 6
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 784
    Top = 248
    Width = 73
    Height = 25
    Caption = #1055#1088#1086#1077#1082#1094#1080#1103
    TabOrder = 7
    OnClick = Button8Click
  end
  object Edit1: TEdit
    Left = 600
    Top = 216
    Width = 73
    Height = 21
    TabOrder = 8
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 784
    Top = 216
    Width = 73
    Height = 21
    TabOrder = 9
    Text = 'Edit2'
  end
  object LabeledEdit1: TLabeledEdit
    Left = 672
    Top = 320
    Width = 121
    Height = 21
    EditLabel.Width = 62
    EditLabel.Height = 13
    EditLabel.Caption = 'LabeledEdit1'
    LabelPosition = lpAbove
    LabelSpacing = 3
    TabOrder = 10
    OnMouseMove = Image1MouseMove
  end
end
