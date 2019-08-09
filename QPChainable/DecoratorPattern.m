//
//  DecoratorPattern.m
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/8/5.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//

#import "DecoratorPattern.h"


@implementation QPConcreteChainableModel

@end


@interface QPConcreteChainableMaker<instancetype, datatype> () {
    __strong QPConcreteChainableModel *_strongData;
    __weak QPConcreteChainableModel *_weakData;
}

@end


@implementation QPConcreteChainableMaker

- (instancetype)initWithData:(QPConcreteChainableModel *)data
{
    self = [super init];
    if (self) {
        if (self == (id)data) {
            _weakData = data;
        }
        else {
            _strongData = data;
        }
    }
    return self;
}

- (QPConcreteChainableModel *)data {
    return _strongData ?: _weakData;
}

- (QPConcreteChainableMaker *)make
{
    return self;
}

@end


@implementation QPChainableModel

@end
