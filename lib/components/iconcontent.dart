import 'package:flutter/material.dart';

//use for widgets WITH icon

class IconContent extends StatelessWidget {

  IconContent({this.icon, this.label});

  final IconData? icon;
  final String? label;

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: <Widget> [
        Icon(
          icon,
          size: 80.0,
        ),
        SizedBox(
          height: 15.0,
        ),
        Text(
          label!,
          style: TextStyle(
            fontSize: 40.0,
            color: Color(0xFFFFFF00),
          ),
        ),
      ],
    );
  }
}