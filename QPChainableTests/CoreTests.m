//
//  CoreTests.m
//  QPChainable
//
//  Created by keqiongpan@163.com on 2020/1/10.
//  Copyright © 2020 Qiongpan Ke. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <QPChainable/QPChainable.h>


#pragma mark - QPPacketMaker

@interface QPPacketMaker : NSObject

@property (nonatomic, strong) NSMutableData *mutableData;

- QP_CHAINABLE_DECLARE(length, (void), NSUInteger);
- QP_CHAINABLE_DECLARE(write, (NSString *format, ...) /* , instancetype */);
- QP_CHAINABLE_DECLARE(append, (NSData *data), QPPacketMaker *);
- QP_CHAINABLE_DECLARE(send, (void), QPPacketMaker *);

@end

@implementation QPPacketMaker

- (instancetype)init{
    if ((self = [super init])) {
        self.mutableData = NSMutableData.new;
    }
    return self;
}

#pragma mark length()
- QP_CHAINABLE_IMPLEMENTATION(length, (void), NSUInteger, ({
    return self.mutableData.length;
}));

#pragma mark write(NSString *format, ...)
- QP_CHAINABLE_IMPLEMENTATION(write, (NSString *format, ...) /* , instancetype */, ({
    va_list arguments;
    va_start(arguments, format);
    NSString *string = [[NSString alloc] initWithFormat:format arguments:arguments];
    va_end(arguments);
    [self.mutableData appendData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}));

#pragma mark append(QPMultipartData *data)
- QP_CHAINABLE_IMPLEMENTATION(append, (NSData *data), QPPacketMaker *, ({
    [self.mutableData appendData:data];
    return self;
}));

#pragma mark send()
- QP_CHAINABLE_IMPLEMENTATION(send, (void), QPPacketMaker *, ({
    self.mutableData = NSMutableData.new;
    return self;
}));

@end


#pragma mark - CoreTests

@interface CoreTests : XCTestCase

@end

@implementation CoreTests

- (void)testSimpleCall {
    QPPacketMaker *maker = QPPacketMaker.new;
    NSData *abc = [@"abc" dataUsingEncoding:NSUTF8StringEncoding];
    XCTAssert(0 == maker.length(), "初始时数据包的长度应为0。");
    XCTAssert(maker == maker.write(@"abc"), "调用write(NSString *format, ...)返回对象自身。");
    XCTAssert(maker == maker.append(abc), "调用append(NSData *data)返回对象自身。");
    XCTAssert(maker == maker.send(), "调用send()返回对象自身。");
}

- (void)testChainedCalls {
    QPPacketMaker *maker = QPPacketMaker.new;
    NSData *abc = [@"abc" dataUsingEncoding:NSUTF8StringEncoding];
    XCTAssert(0 == maker.length(), "初始时数据包的长度应为0。");
    XCTAssert(3 == maker.append(abc).length(), "追加\"abc\"后数据包的长度应为3。");
    XCTAssert(0 == maker.append(abc).send().length(), "数据包发送后的长度应恢复为0。");
}

- (void)testChainedFunctionReturnType {
    QPPacketMaker *maker = QPPacketMaker.new;
    NSData *abc = [@"abc" dataUsingEncoding:NSUTF8StringEncoding];
    XCTAssert(maker == maker.append(abc), "调用append(NSData *data)返回的类型为QPPacketMaker *。");
    XCTAssert(maker == maker.write(@"defg"), "调用write(NSString *format, ...)返回的类型为instancetype。");
    XCTAssert(7 == maker.length(), "调用length()返回的类型为NSUInteger。");
}

- (void)testChainedFunctionArgumentTypes {
    QPPacketMaker *maker = QPPacketMaker.new;
    NSData *abc = [@"abc" dataUsingEncoding:NSUTF8StringEncoding];
    XCTAssert(maker.send(), "调用无参链式函数send(void)时不需要传入任何参数。");
    XCTAssert(maker.append(abc), "调用链式函数append(NSData *data)需要传入一个参数。");
    XCTAssert(maker.write(@"Hi, %@!", @"everyone"), "调用链式函数write(NSString *format, ...)可以传入多个参数。");
}

@end
