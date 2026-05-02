#ifndef CONEXION_H
#define CONEXION_H

// Librerias necesarias para ODBC (conexion a base de datos)
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Variables globales de conexion ODBC
// Se declaran aqui para que todos los archivos
// puedan usarlas al incluir este header.
// =============================================
extern SQLHENV hEnv;   // Ambiente ODBC
extern SQLHDBC hDbc;   // Conexion a la base de datos
extern SQLHSTMT hStmt; // Sentencia para ejecutar SQL

// =============================================
// Declaracion de funciones
// =============================================
bool conectar();
void cerrarConexion();

#endif