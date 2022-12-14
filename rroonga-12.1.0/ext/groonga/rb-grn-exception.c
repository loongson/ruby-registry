/* -*- coding: utf-8; mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
  Copyright (C) 2009-2016  Kouhei Sutou <kou@clear-code.com>
  Copyright (C) 2014-2016  Masafumi Yokoyama <yokoyama@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "rb-grn.h"

VALUE rb_eGrnError;
VALUE rb_eGrnClosed;
VALUE rb_eGrnNoSuchColumn;

static VALUE eGrnEndOfData;
static VALUE eGrnUnknownError;
static VALUE eGrnOperationNotPermitted;
static VALUE eGrnNoSuchFileOrDirectory;
static VALUE eGrnNoSuchProcess;
static VALUE eGrnInterruptedFunctionCall;
static VALUE eGrnInputOutputError;
static VALUE eGrnNoSuchDeviceOrAddress;
static VALUE eGrnArgumentListTooLong;
static VALUE eGrnExecFormatError;
static VALUE eGrnBadFileDescriptor;
static VALUE eGrnNoChildProcesses;
static VALUE eGrnResourceTemporarilyUnavailable;
static VALUE eGrnNotEnoughSpace;
static VALUE eGrnPermissionDenied;
static VALUE eGrnBadAddress;
static VALUE eGrnResourceBusy;
static VALUE eGrnFileExists;
static VALUE eGrnImproperLink;
static VALUE eGrnNoSuchDevice;
static VALUE eGrnNotADirectory;
static VALUE eGrnIsADirectory;
VALUE rb_eGrnInvalidArgument;
static VALUE eGrnTooManyOpenFilesInSystem;
static VALUE eGrnTooManyOpenFiles;
static VALUE eGrnInappropriateIOControlOperation;
static VALUE eGrnFileTooLarge;
static VALUE eGrnNoSpaceLeftOnDevice;
static VALUE eGrnInvalidSeek;
static VALUE eGrnReadOnlyFileSystem;
static VALUE eGrnTooManyLinks;
static VALUE eGrnBrokenPipe;
static VALUE eGrnDomainError;
static VALUE eGrnResultTooLarge;
static VALUE eGrnResourceDeadlockAvoided;
VALUE rb_eGrnNoMemoryAvailable;
static VALUE eGrnFilenameTooLong;
static VALUE eGrnNoLocksAvailable;
static VALUE eGrnFunctionNotImplemented;
static VALUE eGrnDirectoryNotEmpty;
static VALUE eGrnIllegalByteSequence;
static VALUE eGrnSocketNotInitialized;
static VALUE eGrnOperationWouldBlock;
static VALUE eGrnAddressIsNotAvailable;
static VALUE eGrnNetworkIsDown;
static VALUE eGrnNoBuffer;
static VALUE eGrnSocketIsAlreadyConnected;
static VALUE eGrnSocketIsNotConnected;
static VALUE eGrnSocketIsAlreadyShutdowned;
static VALUE eGrnOperationTimeout;
static VALUE eGrnConnectionRefused;
static VALUE eGrnRangeError;
static VALUE eGrnTokenizerError;
static VALUE eGrnFileCorrupt;
static VALUE eGrnInvalidFormat;
static VALUE eGrnObjectCorrupt;
static VALUE eGrnTooManySymbolicLinks;
static VALUE eGrnNotSocket;
static VALUE eGrnOperationNotSupported;
static VALUE eGrnAddressIsInUse;
static VALUE eGrnZLibError;
static VALUE eGrnLZ4Error;
static VALUE eGrnStackOverFlow;
static VALUE eGrnSyntaxError;
static VALUE eGrnRetryMax;
static VALUE eGrnIncompatibleFileFormat;
static VALUE eGrnUpdateNotAllowed;
static VALUE eGrnTooSmallOffset;
static VALUE eGrnTooLargeOffset;
static VALUE eGrnTooSmallLimit;
static VALUE eGrnCASError;
static VALUE eGrnUnsupportedCommandVersion;
static VALUE eGrnNormalizerError;
static VALUE eGrnTokenFilterError;
static VALUE eGrnCommandError;
static VALUE eGrnPluginError;
static VALUE eGrnScorerError;
static VALUE eGrnCancel;
static VALUE eGrnWindowFunctionError;
static VALUE eGrnZstdError;
static VALUE eGrnConnectionReset;

VALUE
rb_grn_rc_to_exception (grn_rc rc)
{
    VALUE exception = Qnil;

    switch (rc) {
      case GRN_SUCCESS:
        return Qnil;
        break;
      case GRN_END_OF_DATA:
        exception = eGrnEndOfData;
        break;
      case GRN_UNKNOWN_ERROR:
        exception = eGrnUnknownError;
        break;
      case GRN_OPERATION_NOT_PERMITTED:
        exception = eGrnOperationNotPermitted;
        break;
      case GRN_NO_SUCH_FILE_OR_DIRECTORY:
        exception = eGrnNoSuchFileOrDirectory;
        break;
      case GRN_NO_SUCH_PROCESS:
        exception = eGrnNoSuchProcess;
        break;
      case GRN_INTERRUPTED_FUNCTION_CALL:
        exception = eGrnInterruptedFunctionCall;
        break;
      case GRN_INPUT_OUTPUT_ERROR:
        exception = eGrnInputOutputError;
        break;
      case GRN_NO_SUCH_DEVICE_OR_ADDRESS:
        exception = eGrnNoSuchDeviceOrAddress;
        break;
      case GRN_ARG_LIST_TOO_LONG:
        exception = eGrnArgumentListTooLong;
        break;
      case GRN_EXEC_FORMAT_ERROR:
        exception = eGrnExecFormatError;
        break;
      case GRN_BAD_FILE_DESCRIPTOR:
        exception = eGrnBadFileDescriptor;
        break;
      case GRN_NO_CHILD_PROCESSES:
        exception = eGrnNoChildProcesses;
        break;
      case GRN_RESOURCE_TEMPORARILY_UNAVAILABLE:
        exception = eGrnResourceTemporarilyUnavailable;
        break;
      case GRN_NOT_ENOUGH_SPACE:
        exception = eGrnNotEnoughSpace;
        break;
      case GRN_PERMISSION_DENIED:
        exception = eGrnPermissionDenied;
        break;
      case GRN_BAD_ADDRESS:
        exception = eGrnBadAddress;
        break;
      case GRN_RESOURCE_BUSY:
        exception = eGrnResourceBusy;
        break;
      case GRN_FILE_EXISTS:
        exception = eGrnFileExists;
        break;
      case GRN_IMPROPER_LINK:
        exception = eGrnImproperLink;
        break;
      case GRN_NO_SUCH_DEVICE:
        exception = eGrnNoSuchDevice;
        break;
      case GRN_NOT_A_DIRECTORY:
        exception = eGrnNotADirectory;
        break;
      case GRN_IS_A_DIRECTORY:
        exception = eGrnIsADirectory;
        break;
      case GRN_INVALID_ARGUMENT:
        exception = rb_eGrnInvalidArgument;
        break;
      case GRN_TOO_MANY_OPEN_FILES_IN_SYSTEM:
        exception = eGrnTooManyOpenFilesInSystem;
        break;
      case GRN_TOO_MANY_OPEN_FILES:
        exception = eGrnTooManyOpenFiles;
        break;
      case GRN_INAPPROPRIATE_I_O_CONTROL_OPERATION:
        exception = eGrnInappropriateIOControlOperation;
        break;
      case GRN_FILE_TOO_LARGE:
        exception = eGrnFileTooLarge;
        break;
      case GRN_NO_SPACE_LEFT_ON_DEVICE:
        exception = eGrnNoSpaceLeftOnDevice;
        break;
      case GRN_INVALID_SEEK:
        exception = eGrnInvalidSeek;
        break;
      case GRN_READ_ONLY_FILE_SYSTEM:
        exception = eGrnReadOnlyFileSystem;
        break;
      case GRN_TOO_MANY_LINKS:
        exception = eGrnTooManyLinks;
        break;
      case GRN_BROKEN_PIPE:
        exception = eGrnBrokenPipe;
        break;
      case GRN_DOMAIN_ERROR:
        exception = eGrnDomainError;
        break;
      case GRN_RESULT_TOO_LARGE:
        exception = eGrnResultTooLarge;
        break;
      case GRN_RESOURCE_DEADLOCK_AVOIDED:
        exception = eGrnResourceDeadlockAvoided;
        break;
      case GRN_NO_MEMORY_AVAILABLE:
        exception = rb_eGrnNoMemoryAvailable;
        break;
      case GRN_FILENAME_TOO_LONG:
        exception = eGrnFilenameTooLong;
        break;
      case GRN_NO_LOCKS_AVAILABLE:
        exception = eGrnNoLocksAvailable;
        break;
      case GRN_FUNCTION_NOT_IMPLEMENTED:
        exception = eGrnFunctionNotImplemented;
        break;
      case GRN_DIRECTORY_NOT_EMPTY:
        exception = eGrnDirectoryNotEmpty;
        break;
      case GRN_ILLEGAL_BYTE_SEQUENCE:
        exception = eGrnIllegalByteSequence;
        break;
      case GRN_SOCKET_NOT_INITIALIZED:
        exception = eGrnSocketNotInitialized;
        break;
      case GRN_OPERATION_WOULD_BLOCK:
        exception = eGrnOperationWouldBlock;
        break;
      case GRN_ADDRESS_IS_NOT_AVAILABLE:
        exception = eGrnAddressIsNotAvailable;
        break;
      case GRN_NETWORK_IS_DOWN:
        exception = eGrnNetworkIsDown;
        break;
      case GRN_NO_BUFFER:
        exception = eGrnNoBuffer;
        break;
      case GRN_SOCKET_IS_ALREADY_CONNECTED:
        exception = eGrnSocketIsAlreadyConnected;
        break;
      case GRN_SOCKET_IS_NOT_CONNECTED:
        exception = eGrnSocketIsNotConnected;
        break;
      case GRN_SOCKET_IS_ALREADY_SHUTDOWNED:
        exception = eGrnSocketIsAlreadyShutdowned;
        break;
      case GRN_OPERATION_TIMEOUT:
        exception = eGrnOperationTimeout;
        break;
      case GRN_CONNECTION_REFUSED:
        exception = eGrnConnectionRefused;
        break;
      case GRN_RANGE_ERROR:
        exception = eGrnRangeError;
        break;
      case GRN_TOKENIZER_ERROR:
        exception = eGrnTokenizerError;
        break;
      case GRN_FILE_CORRUPT:
        exception = eGrnFileCorrupt;
        break;
      case GRN_INVALID_FORMAT:
        exception = eGrnInvalidFormat;
        break;
      case GRN_OBJECT_CORRUPT:
        exception = eGrnObjectCorrupt;
        break;
      case GRN_TOO_MANY_SYMBOLIC_LINKS:
        exception = eGrnTooManySymbolicLinks;
        break;
      case GRN_NOT_SOCKET:
        exception = eGrnNotSocket;
        break;
      case GRN_OPERATION_NOT_SUPPORTED:
        exception = eGrnOperationNotSupported;
        break;
      case GRN_ADDRESS_IS_IN_USE:
        exception = eGrnAddressIsInUse;
        break;
      case GRN_ZLIB_ERROR:
        exception = eGrnZLibError;
        break;
      case GRN_LZ4_ERROR:
        exception = eGrnLZ4Error;
        break;
      case GRN_STACK_OVER_FLOW:
        exception = eGrnStackOverFlow;
        break;
      case GRN_SYNTAX_ERROR:
        exception = eGrnSyntaxError;
        break;
      case GRN_RETRY_MAX:
        exception = eGrnRetryMax;
        break;
      case GRN_INCOMPATIBLE_FILE_FORMAT:
        exception = eGrnIncompatibleFileFormat;
        break;
      case GRN_UPDATE_NOT_ALLOWED:
        exception = eGrnUpdateNotAllowed;
        break;
      case GRN_TOO_SMALL_OFFSET:
        exception = eGrnTooSmallOffset;
        break;
      case GRN_TOO_LARGE_OFFSET:
        exception = eGrnTooLargeOffset;
        break;
      case GRN_TOO_SMALL_LIMIT:
        exception = eGrnTooSmallLimit;
        break;
      case GRN_CAS_ERROR:
        exception = eGrnCASError;
        break;
      case GRN_UNSUPPORTED_COMMAND_VERSION:
        exception = eGrnUnsupportedCommandVersion;
        break;
      case GRN_NORMALIZER_ERROR:
        exception = eGrnNormalizerError;
        break;
      case GRN_TOKEN_FILTER_ERROR:
        exception = eGrnTokenFilterError;
        break;
      case GRN_COMMAND_ERROR:
        exception = eGrnCommandError;
        break;
      case GRN_PLUGIN_ERROR:
        exception = eGrnPluginError;
        break;
      case GRN_SCORER_ERROR:
        exception = eGrnScorerError;
        break;
      case GRN_CANCEL:
        exception = eGrnCancel;
        break;
      case GRN_WINDOW_FUNCTION_ERROR:
        exception = eGrnWindowFunctionError;
        break;
      case GRN_ZSTD_ERROR:
        exception = eGrnZstdError;
        break;
      case GRN_CONNECTION_RESET:
        exception = eGrnConnectionReset;
        break;
    }

    if (NIL_P(exception))
        rb_raise(rb_eGrnError, "invalid return code: %d", rc);

    return exception;
}

void
rb_grn_rc_check (grn_rc rc, VALUE related_object)
{
    VALUE exception;
    const char *message;

    exception = rb_grn_rc_to_exception(rc);
    if (NIL_P(exception))
        return;
    message = grn_rc_to_string(rc);

    if (NIL_P(related_object))
        rb_raise(exception, "%s", message);
    else
        rb_raise(exception, "%s: %s", rb_grn_inspect(related_object),  message);
}

void
rb_grn_init_exception (VALUE mGrn)
{
    /*
     * Document-class: Groonga::Error
     *
     * rroonga????????????????????????????????????????????????
     */
    rb_eGrnError =
        rb_define_class_under(mGrn, "Error", rb_eStandardError);

    /*
     * Document-class: Groonga::Closed
     *
     * groonga??????????????????????????????????????????Ruby?????????????????????
     * ??????????????????????????????????????????????????????????????????
     *
     * @since 1.2.3
     */
    rb_eGrnClosed = rb_define_class_under(mGrn, "Closed", rb_eGrnError);

    /*
     * Document-class: Groonga::ObjectClosed
     *
     * groonga??????????????????????????????????????????Ruby?????????????????????
     * ??????????????????????????????????????????????????????????????????
     *
     * @deprecated Use {Closed} instead since 1.2.3.
     */
    rb_define_const(mGrn, "ObjectClosed", rb_eGrnClosed);

    /*
     * Document-class: Groonga::NoSuchColumn
     *
     * ???????????????????????????????????????????????????????????????
     */
    rb_eGrnNoSuchColumn =
        rb_define_class_under(mGrn, "NoSuchColumn", rb_eGrnError);

    /*
     * Document-class: Groonga::EndOfData
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnEndOfData =
        rb_define_class_under(mGrn, "EndOfData", rb_eGrnError);

    /*
     * Document-class: Groonga::UnknownError
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnUnknownError =
        rb_define_class_under(mGrn, "UnknownError", rb_eGrnError);

    /*
     * Document-class: Groonga::OperationNotPermitted
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnOperationNotPermitted =
        rb_define_class_under(mGrn, "OperationNotPermitted", rb_eGrnError);

    /*
     * Document-class: Groonga::NoSuchFileOrDirectory
     *
     * ????????????????????????????????????????????????????????????????????????
     */
    eGrnNoSuchFileOrDirectory =
        rb_define_class_under(mGrn, "NoSuchFileOrDirectory", rb_eGrnError);

    /*
     * Document-class: Groonga::NoSuchProcess
     *
     * ?????????????????????????????????????????????
     */
    eGrnNoSuchProcess =
        rb_define_class_under(mGrn, "NoSuchProcess", rb_eGrnError);

    /*
     * Document-class: Groonga::InterruptedFunctionCall
     *
     * ????????????????????????????????????????????????????????????
     */
    eGrnInterruptedFunctionCall =
        rb_define_class_under(mGrn, "InterruptedFunctionCall", rb_eGrnError);

    /*
     * Document-class: Groonga::InputOutputError
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnInputOutputError =
        rb_define_class_under(mGrn, "InputOutputError", rb_eGrnError);

    /*
     * Document-class: Groonga::NoSuchDeviceOrAddress
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnNoSuchDeviceOrAddress =
        rb_define_class_under(mGrn, "NoSuchDeviceOrAddress", rb_eGrnError);

    /*
     * Document-class: Groonga::ArgumentListTooLong
     *
     * ???????????????????????????????????????????????????
     */
    eGrnArgumentListTooLong =
        rb_define_class_under(mGrn, "ArgumentListTooLong", rb_eGrnError);

    /*
     * Document-class: Groonga::ExecFormatError
     *
     * ?????????????????????????????????????????????????????????????????????????????????
     */
    eGrnExecFormatError =
        rb_define_class_under(mGrn, "ExecFormatError", rb_eGrnError);

    /*
     * Document-class: Groonga::BadFileDescriptor
     *
     * ???????????????????????????????????????????????????????????????????????????
     */
    eGrnBadFileDescriptor =
        rb_define_class_under(mGrn, "BadFileDescriptor", rb_eGrnError);

    /*
     * Document-class: Groonga::NoChildProcesses
     *
     * ????????????????????????????????????????????????
     */
    eGrnNoChildProcesses =
        rb_define_class_under(mGrn, "NoChildProcesses", rb_eGrnError);

    /*
     * Document-class: Groonga::ResourceTemporarilyUnavailable
     *
     * ?????????????????????????????????????????????????????????????????????
     */
    eGrnResourceTemporarilyUnavailable =
        rb_define_class_under(mGrn, "ResourceTemporarilyUnavailable",
                              rb_eGrnError);

    /*
     * Document-class: Groonga::NotEnoughSpace
     *
     * ?????????????????????????????????????????????
     */
    eGrnNotEnoughSpace =
        rb_define_class_under(mGrn, "NotEnoughSpace", rb_eGrnError);

    /*
     * Document-class: Groonga::PermissionDenied
     *
     * ???????????????????????????????????????
     */
    eGrnPermissionDenied =
        rb_define_class_under(mGrn, "PermissionDenied", rb_eGrnError);

    /*
     * Document-class: Groonga::BadAddress
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnBadAddress =
        rb_define_class_under(mGrn, "BadAddress", rb_eGrnError);

    /*
     * Document-class: Groonga::ResourceBusy
     *
     * ???????????????????????????????????????????????????
     */
    eGrnResourceBusy =
        rb_define_class_under(mGrn, "ResourceBusy", rb_eGrnError);

    /*
     * Document-class: Groonga::FileExists
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnFileExists =
        rb_define_class_under(mGrn, "FileExists", rb_eGrnError);

    /*
     * Document-class: Groonga::ImproperLink
     *
     * ???????????????????????????????????????????????????
     */
    eGrnImproperLink =
        rb_define_class_under(mGrn, "ImproperLink", rb_eGrnError);

    /*
     * Document-class: Groonga::NoSuchDevice
     *
     * ?????????????????????????????????????????????
     */
    eGrnNoSuchDevice =
        rb_define_class_under(mGrn, "NoSuchDevice", rb_eGrnError);

    /*
     * Document-class: Groonga::NotADirectory
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnNotADirectory =
        rb_define_class_under(mGrn, "NotADirectory", rb_eGrnError);

    /*
     * Document-class: Groonga::IsADirectory
     *
     * ?????????????????????????????????????????????
     */
    eGrnIsADirectory =
        rb_define_class_under(mGrn, "IsADirectory", rb_eGrnError);

    /*
     * Document-class: Groonga::InvalidArgument
     *
     * ????????????????????????????????????????????????
     */
    rb_eGrnInvalidArgument =
        rb_define_class_under(mGrn, "InvalidArgument", rb_eGrnError);

    /*
     * Document-class: Groonga::TooManyOpenFilesInSystem
     *
     * ???????????????????????????????????????????????????????????????????????????????????????
     */
    eGrnTooManyOpenFilesInSystem =
        rb_define_class_under(mGrn, "TooManyOpenFilesInSystem", rb_eGrnError);

    /*
     * Document-class: Groonga::TooManyOpenFiles
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnTooManyOpenFiles =
        rb_define_class_under(mGrn, "TooManyOpenFiles", rb_eGrnError);

    /*
     * Document-class: Groonga::InappropriateIOControlOperation
     *
     * IO????????????????????????????????????????????????
     */
    eGrnInappropriateIOControlOperation =
        rb_define_class_under(mGrn, "InappropriateIOControlOperation", rb_eGrnError);

    /*
     * Document-class: Groonga::FileTooLarge
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnFileTooLarge =
        rb_define_class_under(mGrn, "FileTooLarge", rb_eGrnError);

    /*
     * Document-class: Groonga::NoSpaceLeftOnDevice
     *
     * ?????????????????????????????????????????????????????????????????????
     */
    eGrnNoSpaceLeftOnDevice =
        rb_define_class_under(mGrn, "NoSpaceLeftOnDevice", rb_eGrnError);

    /*
     * Document-class: Groonga::InvalidSeek
     *
     * seek??????????????????????????????????????????
     */
    eGrnInvalidSeek =
        rb_define_class_under(mGrn, "InvalidSeek", rb_eGrnError);

    /*
     * Document-class: Groonga::ReadOnlyFileSystem
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnReadOnlyFileSystem =
        rb_define_class_under(mGrn, "ReadOnlyFileSystem", rb_eGrnError);

    /*
     * Document-class: Groonga::TooManyLinks
     *
     * ????????????????????????????????????????????????
     */
    eGrnTooManyLinks =
        rb_define_class_under(mGrn, "TooManyLinks", rb_eGrnError);

    /*
     * Document-class: Groonga::BrokenPipe
     *
     * ???????????????????????????????????????????????????
     */
    eGrnBrokenPipe =
        rb_define_class_under(mGrn, "BrokenPipe", rb_eGrnError);

    /*
     * Document-class: Groonga::DomainError
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnDomainError =
        rb_define_class_under(mGrn, "DomainError", rb_eGrnError);

    /*
     * Document-class: Groonga::ResultTooLarge
     *
     * ?????????????????????????????????????????????
     */
    eGrnResultTooLarge =
        rb_define_class_under(mGrn, "ResultTooLarge", rb_eGrnError);

    /*
     * Document-class: Groonga::ResourceDeadlockAvoided
     *
     * ????????????????????????????????????????????????????????????????????????
     */
    eGrnResourceDeadlockAvoided =
        rb_define_class_under(mGrn, "ResourceDeadlockAvoided", rb_eGrnError);

    /*
     * Document-class: Groonga::NoMemoryAvailable
     *
     * ????????????????????????????????????????????????
     */
    rb_eGrnNoMemoryAvailable =
        rb_define_class_under(mGrn, "NoMemoryAvailable", rb_eGrnError);

    /*
     * Document-class: Groonga::FilenameTooLong
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnFilenameTooLong =
        rb_define_class_under(mGrn, "FilenameTooLong", rb_eGrnError);

    /*
     * Document-class: Groonga::NoLocksAvailable
     *
     * ??????????????????????????????????????????
     */
    eGrnNoLocksAvailable =
        rb_define_class_under(mGrn, "NoLocksAvailable", rb_eGrnError);

    /*
     * Document-class: Groonga::FunctionNotImplemented
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnFunctionNotImplemented =
        rb_define_class_under(mGrn, "FunctionNotImplemented", rb_eGrnError);

    /*
     * Document-class: Groonga::DirectoryNotEmpty
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnDirectoryNotEmpty =
        rb_define_class_under(mGrn, "DirectoryNotEmpty", rb_eGrnError);

    /*
     * Document-class: Groonga::IllegalByteSequence
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnIllegalByteSequence =
        rb_define_class_under(mGrn, "IllegalByteSequence", rb_eGrnError);

    /*
     * Document-class: Groonga::SocketNotInitialized
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnSocketNotInitialized =
        rb_define_class_under(mGrn, "SocketNotInitialized", rb_eGrnError);

    /*
     * Document-class: Groonga::OperationWouldBlock
     *
     * ?????????????????????????????????????????????????????????????????????
     */
    eGrnOperationWouldBlock =
        rb_define_class_under(mGrn, "OperationWouldBlock", rb_eGrnError);

    /*
     * Document-class: Groonga::AddressIsNotAvailable
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnAddressIsNotAvailable =
        rb_define_class_under(mGrn, "AddressIsNotAvailable", rb_eGrnError);

    /*
     * Document-class: Groonga::NetworkIsDown
     *
     * ????????????????????????????????????????????????????????????
     */
    eGrnNetworkIsDown =
        rb_define_class_under(mGrn, "NetworkIsDown", rb_eGrnError);

    /*
     * Document-class: Groonga::NoBuffer
     *
     * ?????????????????????????????????????????????
     */
    eGrnNoBuffer =
        rb_define_class_under(mGrn, "NoBuffer", rb_eGrnError);

    /*
     * Document-class: Groonga::SocketIsAlreadyConnected
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnSocketIsAlreadyConnected =
        rb_define_class_under(mGrn, "SocketIsAlreadyConnected", rb_eGrnError);

    /*
     * Document-class: Groonga::SocketIsNotConnected
     *
     * ???????????????????????????????????????????????????????????????
     */
    eGrnSocketIsNotConnected =
        rb_define_class_under(mGrn, "SocketIsNotConnected", rb_eGrnError);

    /*
     * Document-class: Groonga::SocketIsAlreadyShutdowned
     *
     * ?????????????????????????????????????????????????????????????????????
     */
    eGrnSocketIsAlreadyShutdowned =
        rb_define_class_under(mGrn, "SocketIsAlreadyShutdowned", rb_eGrnError);

    /*
     * Document-class: Groonga::OperationTimeout
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnOperationTimeout =
        rb_define_class_under(mGrn, "OperationTimeout", rb_eGrnError);

    /*
     * Document-class: Groonga::ConnectionRefused
     *
     * ????????????????????????????????????????????????
     */
    eGrnConnectionRefused =
        rb_define_class_under(mGrn, "ConnectionRefused", rb_eGrnError);

    /*
     * Document-class: Groonga::RangeError
     *
     * ????????????????????????????????????
     */
    eGrnRangeError =
        rb_define_class_under(mGrn, "RangeError", rb_eGrnError);

    /*
     * Document-class: Groonga::TokenizerError
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnTokenizerError =
        rb_define_class_under(mGrn, "TokenizerError", rb_eGrnError);

    /*
     * Document-class: Groonga::FileCorrupt
     *
     * ?????????????????????????????????????????????????????????
     */
    eGrnFileCorrupt =
        rb_define_class_under(mGrn, "FileCorrupt", rb_eGrnError);

    /*
     * Document-class: Groonga::InvalidFormat
     *
     * ???????????????????????????????????????????????????????????????
     */
    eGrnInvalidFormat =
        rb_define_class_under(mGrn, "InvalidFormat", rb_eGrnError);

    /*
     * Document-class: Groonga::ObjectCorrupt
     *
     * ???????????????????????????????????????????????????????????????
     */
    eGrnObjectCorrupt =
        rb_define_class_under(mGrn, "ObjectCorrupt", rb_eGrnError);

    /*
     * Document-class: Groonga::TooManySymbolicLinks
     *
     * ??????????????????????????????????????????????????????????????????
     */
    eGrnTooManySymbolicLinks =
        rb_define_class_under(mGrn, "TooManySymbolicLinks", rb_eGrnError);

    /*
     * Document-class: Groonga::NotSocket
     *
     * ????????????????????????????????????????????????
     */
    eGrnNotSocket =
        rb_define_class_under(mGrn, "NotSocket", rb_eGrnError);

    /*
     * Document-class: Groonga::OperationNotSupported
     *
     * ???????????????????????????????????????????????????????????????
     */
    eGrnOperationNotSupported =
        rb_define_class_under(mGrn, "OperationNotSupported", rb_eGrnError);

    /*
     * Document-class: Groonga::AddressIsInUse
     *
     * ???????????????????????????????????????????????????
     */
    eGrnAddressIsInUse =
        rb_define_class_under(mGrn, "AddressIsInUse", rb_eGrnError);

    /*
     * Document-class: Groonga::ZLibError
     *
     * ZLib??????????????????????????????????????????
     */
    eGrnZLibError =
        rb_define_class_under(mGrn, "ZLibError", rb_eGrnError);

    /*
     * Document-class: Groonga::LZ4Error
     *
     * LZ4??????????????????????????????????????????
     */
    eGrnLZ4Error =
        rb_define_class_under(mGrn, "LZ4Error", rb_eGrnError);

    /*
     * Document-class: Groonga::StackOverFlow
     *
     * ??????????????????????????????????????????????????????
     */
    eGrnStackOverFlow =
        rb_define_class_under(mGrn, "StackOverFlow", rb_eGrnError);

    /*
     * Document-class: Groonga::SyntaxError
     *
     * ????????????????????????????????????????????????
     */
    eGrnSyntaxError =
        rb_define_class_under(mGrn, "SyntaxError", rb_eGrnError);

    /*
     * Document-class: Groonga::RetryMax
     *
     * ???????????????????????????????????????????????????????????????
     */
    eGrnRetryMax =
        rb_define_class_under(mGrn, "RetryMax", rb_eGrnError);

    /*
     * Document-class: Groonga::IncompatibleFileFormat
     *
     * ??????????????????????????????????????????????????????????????????????????????????????????
     */
    eGrnIncompatibleFileFormat =
        rb_define_class_under(mGrn, "IncompatibleFileFormat", rb_eGrnError);

    /*
     * Document-class: Groonga::UpdateNotAllowed
     *
     * ?????????????????????????????????????????????????????????????????????????????????????????????
     */
    eGrnUpdateNotAllowed =
        rb_define_class_under(mGrn, "UpdateNotAllowed", rb_eGrnError);

    /*
     * Document-class: Groonga::TooSmallOffset
     *
     * offset?????????????????????????????????????????????
     */
    eGrnTooSmallOffset =
        rb_define_class_under(mGrn, "TooSmallOffset", rb_eGrnError);

    /*
     * Document-class: Groonga::TooLargeOffset
     *
     * offset?????????????????????????????????????????????
     */
    eGrnTooLargeOffset =
        rb_define_class_under(mGrn, "TooLargeOffset", rb_eGrnError);

    /*
     * Document-class: Groonga::TooSmallLimit
     *
     * limit?????????????????????????????????????????????
     */
    eGrnTooSmallLimit =
        rb_define_class_under(mGrn, "TooSmallLimit", rb_eGrnError);

    /*
     * Document-class: Groonga::CASError
     *
     * CAS???Compare and Swap??????????????????????????????????????????
     */
    eGrnCASError = rb_define_class_under(mGrn, "CASError", rb_eGrnError);

    /*
     * Document-class: Groonga::UnsupportedCommandVersion
     *
     * ????????????????????????????????????????????????????????????????????????????????????
     */
    eGrnUnsupportedCommandVersion =
        rb_define_class_under(mGrn, "UnsupportedCommandVersion", rb_eGrnError);

    /*
     * Document-class: Groonga::NormalizerError
     *
     * It is used when a normalizer causes an error.
     *
     * @since 2.1.0
     */
    eGrnNormalizerError =
        rb_define_class_under(mGrn, "NormalizerError", rb_eGrnError);

    /*
     * Document-class: Groonga::TokenFilterError
     *
     * It is used when a token filter causes an error.
     *
     * @since 4.0.6
     */
    eGrnTokenFilterError =
        rb_define_class_under(mGrn, "TokenFilterError", rb_eGrnError);

    /*
     * Document-class: Groonga::CommandError
     *
     * It is used when a command causes an error.
     *
     * @since 5.0.0
     */
    eGrnCommandError =
        rb_define_class_under(mGrn, "CommandError", rb_eGrnError);

    /*
     * Document-class: Groonga::PluginError
     *
     * It is used when a plugin causes an error.
     *
     * @since 5.0.0
     */
    eGrnPluginError =
        rb_define_class_under(mGrn, "PluginError", rb_eGrnError);

    /*
     * Document-class: Groonga::ScorerError
     *
     * It is used when a scorer causes an error.
     *
     * @since 5.0.1
     */
    eGrnScorerError =
        rb_define_class_under(mGrn, "ScorerError", rb_eGrnError);

    /*
     * Document-class: Groonga::Cancel
     *
     * It is used when a processing is canceled.
     *
     * @since 6.0.2
     */
    eGrnCancel =
        rb_define_class_under(mGrn, "Cancel", rb_eGrnError);

    /*
     * Document-class: Groonga::WindowFunctionError
     *
     * It is used when window function causes an error.
     *
     * @since 6.0.4
     */
    eGrnWindowFunctionError =
        rb_define_class_under(mGrn, "WindowFunctionError", rb_eGrnError);

    /*
     * Document-class: Groonga::ZstdError
     *
     * It is used when Zstd causes an error.
     *
     * @since 6.1.3
     */
    eGrnZstdError =
        rb_define_class_under(mGrn, "ZstdError", rb_eGrnError);

    /*
     * Document-class: Groonga::ConnectionReset
     *
     * It is used when connection is reset.
     *
     * @since 10.1.1
     */
    eGrnConnectionReset =
        rb_define_class_under(mGrn, "ConnectionReset", rb_eGrnError);
}
